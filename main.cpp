#include <iostream>
#include <string>
#include<vector>
#include <assert.h>
#include <stdlib.h>
#include<time.h>
#include<unistd.h>
#include <sys/types.h>
#include <signal.h>

#define MazeWidth 80
#define MazeHeight 20


using namespace std;

const string Tile[8]= {"Wall","Floor","Farvin","Garvin","Tharvin","Sally","Gold","Kobold"};
const string Direction[4] = {"North", "South", "East", "West" };
int KoboldHp = 7;
int PlayerHp[4] = {10,10,10,10};
int PlayerScore[4] = {0 , 0 , 0 ,0};
int input = 0;
string deadName;
int count[4] = {0 , 0 ,0 ,0};
int turn = 0;


class Maze{

public:
  vector<string> data;
  
  Maze():data(){}
  Maze(string value){
    for(int i = 0; i < MazeWidth* MazeHeight; i++)
    data.push_back(value);
  }

  bool IsXInBounds(int x) const
  {
    return x >= -1 && x <= MazeWidth;
  }

    bool IsYInBounds(int y) const
  {
    return y >= -1 && y <= MazeHeight;
  }

    void SetCell(int x, int y, string celltype)
  {
    assert(IsXInBounds(x));
    assert(IsYInBounds(y));
  
    data[x + MazeWidth * y] = celltype;
  }

  string GetCell(int x, int y) const
  {
     assert(IsXInBounds(x));
     assert(IsYInBounds(y));
  
    return data[x + MazeWidth * y];
  }


  bool IsFloor(int x, int y)
  {
    assert(IsXInBounds(x) && IsYInBounds(y));
  
        if (GetCell(x, y) != Tile[1])
        {
          return false;
	}
  
    return true;
  }


void Print() const
  {
    for (int y = 0; y != MazeHeight; y++)
    {
      for (int x = 0; x != MazeWidth; x++)
      {
	string celltype = GetCell(x, y);
	if(celltype == "Wall") 
          cout << "X";
	else if(celltype == "Floor")
	  cout<< " ";
	else if(celltype == "Farvin")
	  cout<< "1";
	else if(celltype == "Garvin")
	  cout<< "2";
	else if(celltype == "Tharvin")
	  cout<< "3";
	else if(celltype == "Sally")
	  cout<< "4";
	else if(celltype == "Gold")
	  cout<< "G";
	else if(celltype == "Kobold")
	  cout<< "k";
	else cout<<"E";
      }
  
     cout << endl;
    }  
    cout << endl;
  }
};



class MazeGenerator{


public:


  string GetRandomDirection() const{
    int i;    
    i = rand() % 4;
    return Direction[i];
  }
  
  bool MakeWall(Maze& maze, int x, int y, string direction){
    assert(x >= 0 && x < MazeWidth);
    assert(y >= 0 && y < MazeHeight);

    int length = rand() % MazeWidth;

    int xStart = x;
    int yStart = y;
  
    int xEnd = x;
    int yEnd = y;

    if (direction == "North")
      yStart = y - length;
    else if (direction == "East")
      xEnd = x + length;
    else if (direction == "South")
      yEnd = y + length;
    else if (direction == "West")
      xStart = x - length;
    
    if(!maze.IsXInBounds(xStart) || !maze.IsXInBounds(xEnd) || !maze.IsYInBounds(yStart) || !maze.IsYInBounds(yEnd))
      return false;

    for (int i = xStart; i <= xEnd; i++){
      for(int j = yStart; j <= yEnd; j++ ){
	if(!maze.IsFloor(i , j))
	  return false;
      }
    }   
     for (int i = xStart; i <= xEnd; i++){
      for(int j = yStart; j <= yEnd; j++ ){
	    maze.SetCell(i , j , Tile[0]);
      }
    }
     return true;
  }

Maze MakePlayer(Maze& maze, string Name){    
   while(1){ 
    int x, y;
   
    x = rand() % MazeWidth;
    y = rand() % MazeHeight;
    
    if(maze.IsFloor(x , y)){
        if( Name== "Farvin")
          {maze.SetCell(x , y , Tile[2]);
            return maze;}
        else if(Name== "Garvin"){
          maze.SetCell(x , y , Tile[3]);
            return maze;}
        else if(Name== "Tharvin"){
          maze.SetCell(x , y , Tile[4]);
            return maze;}
        else if(Name== "Sally"){
          maze.SetCell(x , y , Tile[5]);
            return maze;}
        else return maze;}    
    }
}

Maze MakeGold(Maze& maze){    
   while(1){ 
    int x, y;
   
    x = rand() % MazeWidth;
    y = rand() % MazeHeight;
    
    if(maze.IsFloor(x , y)){      
          maze.SetCell(x , y , Tile[6]);
            return maze;}  
    }
}

static Maze MakeKobold(Maze& maze){    
   while(1){ 
    int x, y;  
    x = rand() % MazeWidth;
    y = rand() % MazeHeight;    
    if(maze.IsFloor(x , y)){      
          maze.SetCell(x , y , Tile[7]);
            return maze;}  
    }
}

Maze Generate(){
   Maze maze(Tile[1]);
   int maxWall = rand() % 100;
   for(int i = 0; i < maxWall; i++)
    {
     int x = rand() % MazeWidth;
     int y = rand() % MazeHeight;
     string direction = GetRandomDirection();
     MakeWall(maze, x, y, direction);
    }
    
    for(int i =2; i< 6; i++){
     maze = MakePlayer(maze, Tile[i]);
    }
    maze = MakeGold(maze);
    maze = MakeKobold(maze);
     return maze;   
  }
};



class Player
{
private:
  int id;
public:
  int hp;
  int score;

  Player(int _id, int _score):id(_id), score(_score){
    hp = 10;
  }

  string GetName()
  {
    assert(id<5);
    string Name;
    Name = Tile[id + 1];
    return Name;    
  }

  int GetPosition(Maze& maze){
    
    string Name;
    Name = GetName();
    for(int i = 0; i <  MazeWidth*MazeHeight;i++){
      if(maze.data[i] == Name)
	return i;
    }
  }

  void Print(Maze& maze){
    string Name;
    int position;
    Name = GetName();
    position = GetPosition(maze);
    int x, y;    
    x= position % MazeWidth;
    y= position / MazeWidth;
    
    cout<< Name <<"'s hp is "<<PlayerHp[id-1] <<", his score is "<<PlayerScore[id-1]<<", he is at position"<<"("<<x<<","<<y<<") now."<<endl;
  }
};

class Kobold{
public:
  int hp;

  Kobold(){
    hp = 7;
  }

   int GetPosition(Maze& maze){
    string Name;
    Name = Tile[7];
    for(int i = 0; i <  MazeWidth*MazeHeight;i++){
      if(maze.data[i] == Name)
	return i;
    }
  }

   void Print(Maze& maze){
    string Name;
    int position;
    Name = Tile[7];
    position = GetPosition(maze);
    int x, y;    
    x= position % MazeWidth;
    y= position / MazeWidth;
    cout<< Name <<"'s hp is "<<KoboldHp<<", it is at position"<<"("<<x<<","<<y<<") now."<<endl;
  } 
};


class Gold{
public:

  Gold(){}
  
   int GetPosition(Maze& maze){
    string Name;
    Name = Tile[6];
    for(int i = 0; i <  MazeWidth*MazeHeight; i++){
      if(maze.data[i] == Name)
	return i;
    }
  }

   void Print(Maze& maze){
    string Name;
    int position;
    Name = Tile[6];
    position = GetPosition(maze);
    int x, y;    
    x= position % MazeWidth;
    y= position / MazeWidth;
    cout<< Name <<" is at position"<<"("<<x<<","<<y<<") now."<<endl;
  } 
};




class Combat{
public:
  MazeGenerator PGenerator;
  vector<Player> player;
  Kobold kobold;
  Gold gold;
  Combat(Player play1, Player play2, Player play3, Player play4, Kobold _kobold, Gold _gold): kobold(_kobold), gold(_gold){
    player.push_back(play1);
    player.push_back(play2);
    player.push_back(play3);
    player.push_back(play4);
  }


  int direction(){
    int i;
    i = rand() % 4;
    return i;
  }


 bool PlayerMove(int playi, int direction, Maze& maze){
    
    int position;
    position = player[playi - 1].GetPosition(maze);
    int x, y;    
    x= position % MazeWidth;
    y= position / MazeWidth;

    int xEnd, yEnd;
    string Name;
    Name = player[playi - 1].GetName();
     
    if(direction == 0 && x+1 <= MazeWidth) {
      xEnd = x+1;
      maze.SetCell(x , y , Tile[1]);
      maze.SetCell(xEnd , y , Name);
      return true;
    }
    else if(direction == 1 && x-1 >= 0){
      xEnd = x-1;
      maze.SetCell(x , y , Tile[1]);
      maze.SetCell(xEnd , y , Name);
      return true;
    }
     else if(direction == 2 && y-1 >= 0){
       yEnd = y-1;
       maze.SetCell(x , y , Tile[1]);
       maze.SetCell(x , yEnd , Name);
       return true;
    }
     else if (direction == 3 && y+1 <= MazeHeight){
       yEnd = y+1;
       maze.SetCell(x , y , Tile[1]);
       maze.SetCell(x , yEnd , Name);
       return true;
     }   
     else return false;
  }

bool PlayerMeetKobold(int playi, Maze& maze){

    int position;
    position = player[playi - 1].GetPosition(maze);
    int x, y;    
    x= position % MazeWidth;
    y= position / MazeWidth;

    return ( maze.GetCell(x, y) == Tile[7]);
}

bool PlayerMeetGold(int playi, Maze& maze){

    int position;
    position = player[playi - 1].GetPosition(maze);
    int x, y;    
    x= position % MazeWidth;
    y= position / MazeWidth;

    return ( maze.GetCell(x, y) == Tile[6]);
}

bool PlayerMeetWall(int playi, int direction, Maze& maze){

    int position;
    position = player[playi - 1].GetPosition(maze);
    int x, y;    
    x= position % MazeWidth;
    y= position / MazeWidth;

    if(direction == 0) {
      if(x+1 == MazeWidth) return true;
      else return (maze.GetCell(x+1 , y) == Tile[0]);
    }
    else if(direction == 1){
      if(x-1 == -1) return true;
      else return (maze.GetCell(x-1 , y) == Tile[0]);
      }			 
	
     else if(direction == 2){
       if(y-1 == -1) return true;
      else return (maze.GetCell(x , y-1) == Tile[0]);
    }
     else if (direction == 3){
       if(y+1 == MazeHeight) return true;
       else return (maze.GetCell(x , y+1) == Tile[0]);
    }
     else return false; 
  }


bool PlayerMeetGold(int playi, int direction, Maze& maze){

    int position;
    position = player[playi - 1].GetPosition(maze);
    int x, y;    
    x= position % MazeWidth;
    y= position / MazeWidth;
    if(direction == 0) {
      if(x+1 == MazeWidth) return true;
      else return (maze.GetCell(x+1 , y) == Tile[6]);
    }
    else if(direction == 1){
      if(x-1 == -1) return true;
      else return (maze.GetCell(x-1 , y) == Tile[6]);
      }			 
	
     else if(direction == 2){
       if(y-1 == -1) return true;
      else return (maze.GetCell(x , y-1) == Tile[6]);
    }
     else if (direction == 3){
       if(y+1 == MazeHeight) return true;
       else return (maze.GetCell(x , y+1) == Tile[6]);
    }
     else return false; 
  }


bool PlayerMeetKobold(int playi, int direction, Maze& maze){

    int position;
    position = player[playi - 1].GetPosition(maze);
    int x, y;    
    x= position % MazeWidth;
    y= position / MazeWidth;
    if(direction == 0) {
      if(x+1 == MazeWidth) return true;
      else return (maze.GetCell(x+1 , y) == Tile[7]);
    }
    else if(direction == 1){
      if(x-1 == -1) return true;
      else return (maze.GetCell(x-1 , y) == Tile[7]);
      }			 
	
     else if(direction == 2){
       if(y-1 == -1) return true;
      else return (maze.GetCell(x , y-1) == Tile[7]);
    }
     else if (direction == 3){
       if(y+1 == MazeHeight) return true;
       else return (maze.GetCell(x , y+1) == Tile[7]);
    }
     else return false; 
  }


  void PlayerVSKobold(int playi, Maze maze){
      int hit = rand() % 6 + 1;
       KoboldHp = KoboldHp - hit;
       if(KoboldHp <= 0 || PlayerHp[playi-1] <=0 ) {
	 return;}
       else {
	 PlayerHp[playi-1] --;
	 PlayerVSKobold(playi, maze);
       }
  }

  bool PlayerMeetPlayer(int playi, int direction, Maze& maze){
    int position;
    position = player[playi - 1].GetPosition(maze);
    Player playCurrent = player[playi-1];
    Player play1 = player[0];
    Player play2 = player[1];
    Player play3 = player[2];
    Player play4 = player[3];
    int x, y;    
    x= position % MazeWidth;
    y= position / MazeWidth;
    if(direction == 0) {
      
       if (maze.GetCell(x+1, y) == Tile[2])
	 {
	   Playerfight (playCurrent, play1, maze, playi-1, 0);

	 return true;
	 }
      else if (maze.GetCell(x+1, y) == Tile[3])
	{
	  Playerfight (playCurrent, play2, maze, playi-1, 1);
;
	return true;
	}
      else if (maze.GetCell(x+1, y) == Tile[4])
	{
	  Playerfight (playCurrent, play3, maze, playi-1, 2);

	return true;
	}
      else if (maze.GetCell(x+1, y) == Tile[5])
	{
	  Playerfight (playCurrent, play4, maze, playi-1, 3);

	return true;
	}
      
    }
    else if(direction == 1){
    
       if (maze.GetCell(x-1, y) == Tile[2])
	 {
	   Playerfight (playCurrent, play1,maze, playi-1, 0);

	return true;
	}
      else if (maze.GetCell(x-1, y) == Tile[3])
	{
	  Playerfight (playCurrent, play2,maze, playi-1, 1);

	return true;
	}
      else if (maze.GetCell(x-1, y) == Tile[4])
	{
	  Playerfight (playCurrent, play3,maze, playi-1, 2);

	return true;
	}
      else if (maze.GetCell(x-1, y) == Tile[5])
	{
	  Playerfight (playCurrent, play4,maze, playi-1, 3);
	 
	return true;
	}
 
    }			 
	
    else if(direction == 2){
    
       if (maze.GetCell(x, y-1) == Tile[2])
	 {
	   Playerfight (playCurrent, play1,maze, playi-1, 0);
	 
	   return true;
	 }
      else if (maze.GetCell(x, y-1) == Tile[3])
	{
	  Playerfight (playCurrent, play2,maze, playi-1, 1);
	 
	   return true;
	}
      else if (maze.GetCell(x, y-1) == Tile[4])
	{
	  Playerfight (playCurrent, play3,maze, playi-1, 2);
	  
	   return true;
	}
      else if (maze.GetCell(x, y-1) == Tile[5])
	{
	  Playerfight (playCurrent, play4,maze, playi-1, 3);

	   return true;
	}
     
    }
    else if (direction == 3){
      
      if (maze.GetCell(x, y+1) == Tile[2])
	{
	  Playerfight (playCurrent, play1,maze, playi-1, 0);

	  return true;
	}
      else if (maze.GetCell(x, y+1) == Tile[3])
	{
	  Playerfight (playCurrent, play2,maze, playi-1, 1);

	  return true;
	}
      else if (maze.GetCell(x, y+1) == Tile[4])
	{
	  Playerfight (playCurrent, play3,maze, playi-1, 2);

	  return true;
	}
      else if (maze.GetCell(x, y+1) == Tile[5])
	{
	  Playerfight (playCurrent, play4,maze, playi-1, 3);

	  return true;
	}
    }
    else return false; 
  }

  void Playerfight (Player play1, Player play2, Maze& maze, int playi1, int playi2) {
    
    
    if (turn == 0) {
      int temp = PlayerHp[playi2];
      int hit = rand() % 6 + 1;
      PlayerHp[playi2] = temp - hit;
      turn++;
      if (PlayerHp[playi2] <= 0)
	{
	  PlayerScore[playi1] = PlayerScore[playi1]+PlayerScore[playi2];
	  PlayerScore[playi2] = 0;
	  PlayerHp[playi2] = 0;
	  deadName = play2.GetName();
	  int position2;
	  position2 = play2.GetPosition(maze);
	  int x2, y2;    
	  x2= position2 % MazeWidth;
	  y2= position2 / MazeWidth;
	  maze.SetCell(x2 , y2 , Tile[1]);
	  count[playi2]++;
	  return;
	}
       
    }
    else if (turn == 1)
      {
	int temp2 = PlayerHp[playi1];
	int hit2 = rand() % 6 + 1;
	PlayerHp[playi1] = temp2 - hit2;
	turn = 0;
	if (PlayerHp[playi1] <= 0)
	  {
            
	    PlayerScore[playi2] = PlayerScore[playi2]+PlayerScore[playi1];
	    PlayerScore[playi1] = 0;
	    PlayerHp[playi1] = 0;
	    deadName = play1.GetName();
	    int position1;
	    position1 = play1.GetPosition(maze);
	    int x1, y1;    
	    x1= position1 % MazeWidth;
	    y1= position1 / MazeWidth;
	    maze.SetCell(x1 , y1 , Tile[1]);
	    count[playi1]++;
	    return;
	  }
      
      }
    Playerfight (play1, play2, maze, playi1, playi2); 
  }

  

  
};









MazeGenerator* pGenerator = new MazeGenerator();

Maze maze = pGenerator ->Generate();

Player Farvin(1,0);

Player Garvin(2,0);

Player Tharvin(3,0);

Player Sally(4,0);

Kobold kobold;

Gold gold;

Combat com(Farvin, Garvin, Tharvin, Sally,kobold, gold);


void reset (){
  MazeGenerator* pGenerator = new MazeGenerator();
  Maze maze = pGenerator ->Generate();
  Player Farvin(1,0);
  Player Garvin(2,0);
  Player Tharvin(3,0);
  Player Sally(4,0);
  Kobold kobold;
  Gold gold;
  Farvin.Print(maze);
  Garvin.Print(maze);
  Tharvin.Print(maze);
  Sally.Print(maze);
  kobold.Print(maze);
  gold.Print(maze);
  maze.Print();
  Combat com(Farvin, Garvin, Tharvin, Sally,kobold, gold);
  pid_t getpid(void);
  int t = getpid();
  cout << "PID is: "<<t;
}

void my_handler(int signum) {
  cout<<"Interrupt signal (" <<signum<<") received. \n";
  Farvin.Print(maze);
  Garvin.Print(maze);
  Tharvin.Print(maze);
  Sally.Print(maze);
  kobold.Print(maze);
  gold.Print(maze);
  exit(1);
}

void CatchSigSur1(int signum){
  
  cout<<"SIGUSR1 received. \n";
  maze.Print();
   Farvin.Print(maze);
   Garvin.Print(maze);
  Tharvin.Print(maze);
   Sally.Print(maze);
}

void CatchSigSur2(int signum){
cout<<"SIGUSR2 received. \n";
reset();
}






int main(){

 
  
  srand(time(NULL));
  int timeDelay = 0;
  cout << "Please enter the time delay you want (second): ";
  cin >> timeDelay;
  timeDelay = timeDelay * 1000000;


   
  MazeGenerator* pGenerator = new MazeGenerator();

  Maze maze = pGenerator ->Generate();

  Player Farvin(1,0);

  Player Garvin(2,0);

  Player Tharvin(3,0);

  Player Sally(4,0);

  Kobold kobold;

  Gold gold;

  Combat com(Farvin, Garvin, Tharvin, Sally,kobold, gold);
   
   
  Farvin.Print(maze);
  Garvin.Print(maze);
  Tharvin.Print(maze);
  Sally.Print(maze);
  kobold.Print(maze);
  gold.Print(maze);
  maze.Print();



  int dir1, dir2, dir3, dir4;
  dir1 = rand() % 4;
  dir2 = rand() % 4;
  dir3 = rand() % 4;
  dir4 = rand() % 4;
  
  while(1){
    
    usleep(timeDelay);
    
    
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);      
    signal(SIGUSR1, CatchSigSur1);      
    signal(SIGUSR2, CatchSigSur2);
    if(count[0] == 0){
      if((com.PlayerMeetWall(1, dir1, maze))){
	dir1 = (dir1+1) % 4;
      } 
      if((com.PlayerMeetGold(1, dir1, maze))){
	PlayerScore[0] += 10;
	pGenerator-> MakeGold(maze);
      } 
       
      if((com.PlayerMeetKobold(1, dir1, maze))){
	com.PlayerVSKobold(2, maze);
	if( KoboldHp <= 0 ){
	  int position;
	  position = kobold.GetPosition(maze);
	  int x, y;    
	  x= position % MazeWidth;
	  y= position / MazeWidth;
	  maze.SetCell(x , y , Tile[1]);
	  KoboldHp = 7;
	  PlayerScore[1] += 25;
	  pGenerator->MakeKobold(maze);
	}
	if(PlayerHp[0]<=0) {
	  count[0]++;
	  deadName = "Farvin";	  
	  PlayerScore[0] = 0;
	}      
      }


      if((com.PlayerMeetPlayer(1, dir1, maze))){  }
       
      if(count[0] == 0)  com.PlayerMove(1, dir1, maze);

      
    }

    else if (count[0] < 10) {
      int position = 0;
      position = Farvin.GetPosition(maze);
      int x, y;    
      x= position % MazeWidth;
      y= position / MazeWidth;
      maze.SetCell(x , y , Tile[1]);
      count[0]++;
    }
    else if(count[0] == 10) {
      count[0] = 0;
      string playername = deadName;
      pGenerator ->MakePlayer(maze, playername);
      PlayerHp[0] = 10;
    }




    



    
    if(count[1] == 0){
      if((com.PlayerMeetWall(2, dir2, maze))){
	dir2 = (dir2+1) % 4;
      } 
      
      if((com.PlayerMeetGold(2, dir2, maze))){
	PlayerScore[1] += 10;
	pGenerator-> MakeGold(maze);
      } 
      if((com.PlayerMeetKobold(2, dir2, maze))){
	com.PlayerVSKobold(2, maze);
	if( KoboldHp <= 0 ){
	  int position;
	  position = kobold.GetPosition(maze);
	  int x, y;    
	  x= position % MazeWidth;
	  y= position / MazeWidth;
	  maze.SetCell(x , y , Tile[1]);
	  KoboldHp = 7;
	  PlayerScore[1] += 25;
	 
	  pGenerator->MakeKobold(maze);
	}
	if(PlayerHp[1]  <= 0){
	  count[1]++;
	  deadName = "Garvin";	  
	  PlayerScore[1] = 0;
	}       
      }

      if((com.PlayerMeetPlayer(2, dir2, maze))){
      }
      if(count[1] == 0) com.PlayerMove(2, dir2, maze);
       
    }
    else if (count[1] < 10) {
      int position = 0;
      position = Garvin.GetPosition(maze);
      int x, y;    
      x= position % MazeWidth;
      y= position / MazeWidth;
      maze.SetCell(x , y , Tile[1]);
      count[1]++;
    }
    else if(count[1] == 10) {
      count[1] = 0;
      string playername = deadName;
      pGenerator ->MakePlayer(maze, playername);
      PlayerHp[1]=10;
    }







    

    if(count[2] == 0){
      if((com.PlayerMeetWall(3, dir3, maze))){
	dir3 = (dir3+1) % 4;
      } 
      if((com.PlayerMeetGold(3, dir3, maze))){
	PlayerScore[2] += 10;
	pGenerator-> MakeGold(maze);
      } 

      if((com.PlayerMeetKobold(3, dir3, maze))){
	com.PlayerVSKobold(3, maze);
	if( KoboldHp <= 0 ){
	  int position;
	  position = kobold.GetPosition(maze);
	  int x, y;    
	  x= position % MazeWidth;
	  y= position / MazeWidth;
	  maze.SetCell(x , y , Tile[1]);
	  KoboldHp = 7;
	  PlayerScore[2] += 25;
	  pGenerator->MakeKobold(maze);
	}
	if(PlayerHp[2]  <= 0){
	  count[2]++;
	  deadName = "Tharvin";
	  PlayerScore[2] = 0;
	}     
      }
      
      if((com.PlayerMeetPlayer(3, dir3, maze))){    }
     
      if(count[2] == 0)  com.PlayerMove(3, dir3, maze);   

    }
    
    else if (count[2] < 10){
      int position = 0;
      position = Tharvin.GetPosition(maze);
      int x, y;    
      x= position % MazeWidth;
      y= position / MazeWidth;
      maze.SetCell(x , y , Tile[1]);
      count[1]++;
    }
    else if(count[2] == 10) {
      count[2] = 0;
      string playername = deadName;
      pGenerator ->MakePlayer(maze, playername);
      PlayerHp[2]=10;
    }









    
    
    if(count[3]==0){

      if((com.PlayerMeetWall(4, dir4, maze))){
	dir4 = (dir4+1) % 4;
      }


  
      if((com.PlayerMeetGold(4, dir4, maze))){
	PlayerScore[3] += 10;
	pGenerator-> MakeGold(maze);
      }

      if((com.PlayerMeetKobold(4, dir4, maze))){
	com.PlayerVSKobold(4, maze);
	if( KoboldHp <= 0 ){
	  int position;
	  position = kobold.GetPosition(maze);
	  int x, y;    
	  x= position % MazeWidth;
	  y= position / MazeWidth;
	  maze.SetCell(x , y , Tile[1]);
	  KoboldHp = 7;
	  PlayerScore[3] += 25;
	  pGenerator->MakeKobold(maze);
	}
	if(PlayerHp[3]  <= 0){
	  count[3]++;
	  deadName = "Sally";
	  PlayerScore[3] = 0;
	}
      }
       
      if((com.PlayerMeetPlayer(4, dir4, maze))){
      }

      if(count[3] == 0)  com.PlayerMove(4, dir4, maze);
    }

    else if (count[3] < 10) {
      int position = 0;
      position = Sally.GetPosition(maze);
      int x, y;    
      x= position % MazeWidth;
      y= position / MazeWidth;
      maze.SetCell(x , y , Tile[1]);
      count[3]++;
    }
    else if(count[3] == 10) {
      count[3] = 0;
      string playername = deadName;
      pGenerator ->MakePlayer(maze, playername);
      PlayerHp[3] = 10;
    }
  
 

    maze.Print();

  }
  
   



  return 0;
}
