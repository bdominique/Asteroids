#include <iostream>
#include <conio.h>
#include <windows.h>
#include <dos.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
using namespace std;


HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;


void gotoxy(int x, int y){
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void WelcomeScreen(){
    system("cls");
    cout<< "//////////////////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
    cout<< "////////////////////////////////////////////                  ////////////////////////////////////////////////////" << endl;
    cout<< "///////////////////////////////////////////     ASTEROIDS    /////////////////////////////////////////////////////" << endl;
    cout<< "//////////////////////////////////////////                  //////////////////////////////////////////////////////" << endl;
    cout<< "//////////////////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout << " " << endl;
    cout<<"\tWelcome to Asteroids! Use the Arrow keys to move Left and Right. Press the Spacebar at any time to pause."<< endl;
    cout<<"\n\t\t\t\t\tGuide Your Spaceship past Asteroids \n\t\t\t and Lasers while you try to pick up Powerups to boost your score." << endl;
    cout<<"\n\t\t\t\t\tPress the Spacebar to Begin."<< endl;
    cout<<"\n\t\t\t (Tip: For a Smoother Experience, make this Window Fullscreen.)"<< endl;
        }

void GameOver(vector<int>& scorelist, vector<string>& namelist, int newscore){

    system("cls");
    cout<< "\n\n\n\n\n\n\n////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
    cout<< "///////////////////////////////////////////////////                  ///////////////////////////////////////////////////" << endl;
    cout<< "//////////////////////////////////////////////////     GAME OVER    ////////////////////////////////////////////////////" << endl;
    cout<< "/////////////////////////////////////////////////                  /////////////////////////////////////////////////////" << endl;
    cout<< "////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
    cout<<"\t\t\t\t\t\tPress R to continue." << endl;
    Sleep(1500);

    while(getch() != 'r'){}

    int position = 6;
    for(int i = 0; i<scorelist.size(); i++){
        if(newscore > scorelist[i]){
            position = i;
            break;
        }
    }

    int prev_score = 0;
    string prev_name = "";
    for(int i = scorelist.size()-1; i>position; i--){
        scorelist[i] = scorelist[i-1];
        scorelist[i-1] = 0;
        namelist[i] = namelist[i-1];
        namelist[i-1] = "";
    }

    if(position != 6){
        scorelist[position] = newscore;
        cout<< "\t\t\t\tYou Got a High Score! Enter your name and then press Period: ";
        char op;
        string newname = "";
        while (op != '.')
        {
            op = getch();
            cout<<op;
            if(op == 127){
            newname = newname.substr(0, newname.size()-1);
            cout<<newname << "";
                }
            else newname+=op;
        }
        namelist[position] = newname.substr(0, newname.size()-1);
    }
    gotoxy(55,14);
    cout<< "LEADERBOARD";

    for(int i = 0; i<scorelist.size(); i++){
        gotoxy(50,i+15);
        cout << namelist[i] << endl;
        gotoxy(65,i+15);
        cout <<  scorelist[i] << endl;
    }
    cout<<"\n\t\t\t\tPress 'R' to Start Over, or press the Spacebar to Exit."<< endl;
}



//set the point on the std console that we're going to output to in all of our Draw() functions
void setcursor(bool visible, DWORD size){
    if(size ==0){
        size = 20;
    }
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}


//Anything that needs to be outputted onto the std console will be a Point class
class Point{
    private:
        int x;
        int y;
        int delaycounter;
    public:
        bool hit;
        int score;
        Point(){
            x = 10;
            y = 10;
            score = 100;
            delaycounter = 0;
            hit = false;
        }
        Point(int x, int y){
            this->x = x;
            this->y = y;
            score = 100;
            hit = false;
        }
        void SetPoint(int x, int y){
            this->x = x;
            this-> y = y;
        }
        int get_x(){
            return x;
        }
        int get_y(){
            return y;
        }


        //initial Draw function used for anything that only takes 1 character
        void Draw(char ch = 'O'){
            gotoxy(x,y);
            cout <<ch << endl;
        }

        //warn the player about the location of the next asteroid
        void DrawWarning(int row, int j){
            gotoxy(x,y);
            if (row == 1) cout << "WARNING: New Asteroid in Left Row in "<< 5-j << "..."<< endl;
            if (row == 2) cout << "WARNING: New Asteroid in Middle Row in "<< 5-j << "..."<< endl;
            if (row == 3) cout << "WARNING: New Asteroid in Right Row in "<< 5-j << "..."<< endl;
        }

        //remove the warning we just gave
        void EraseWarning(){
            gotoxy(x,y);
            cout << "                                              "<< endl;
        }



        /*
        Draw the plane onto the std console
        (Note: this should be the last thing you draw onto the console
         to avoid something being written over it).
         */

        void DrawPlane(){
            gotoxy(x,y);
            cout << '/' << '\\';
            gotoxy(x-1,y+1);
            cout<< "/  " << '\\';
            gotoxy(x-2,y+2);
            cout<<  "/ /" << '\\' << ' ' << '\\' << endl;
            gotoxy(x-3,y+3);
            cout<<  "/_/  " << "\\_\\"<< endl;
        }


        //if we move left or right, delete the previous location of the plane
        void EraseOldPlane(int x, int y){
            gotoxy(x,y);
            cout << " " << " ";
            gotoxy(x-1,y+1);
            cout<< "   " << " ";
            gotoxy(x-2,y+2);
            cout<<  "   " << " " << ' ' << " " << endl;
            gotoxy(x-3,y+3);
            cout<<  "    " << "    "<< endl;
        }

        //draw the asteroids onto the std console
        void DrawAsteroid(char aray[5] = "OOOOO"){

            gotoxy(x-1,y);
            cout<< aray;
        }

        /*
        this allows asteroids to be drawn, but with a delay so that they
        all don't get drawn at the same y coordinate. Also randomly generates a
        new column for the asteroid to appear in once it reaches the bottom of the screen.
        The first asteroid has a delay of 0 and always copies the spaceship's row, so it doesn't need to randomly
        generate a new row.
        */
        void DrawAsteroidat(int i,int delay, int& location, bool redo = true){

            if(i<delay && this->delaycounter == 0) return;
            if(i == delay) this->delaycounter = 0;
            if(redo == false && this->delaycounter == 0){
                    this->SetPoint(location,0);
                    return;
            }


            this->SetPoint(location, this->delaycounter);


            this->DrawAsteroid();
            this->EraseOldAsteroid();

            this->delaycounter += 1;

            if( this->delaycounter == 29 && delay != 0){
                this->delaycounter = 0;
                this->EraseOldAsteroidat(this->get_x(), this->get_y());
                location = rand() % 3 ;
                if(location == 0) {location = 50;}
                if(location == 1) {location = 60;}
                if(location == 2) {location = 70;}

            }

        }


        //lock the score into one position and output it
        void DrawPlaneScore(){

            gotoxy(20,14);
            cout<< "Score: " << score << endl;

        }

        void DrawLevel(int level){

            gotoxy(20,13);
            cout<< "Level: " << level << endl;

        }


        /*
        after the asteroid has been drawn to a new spot 1 lower on the console, return to its previous location
        and delete it. Make sure that at the bottom an top of the console there are no lingering asteroid pieces also.
        */
        void EraseOldAsteroid(){



            gotoxy(x-1,y-1);
            cout<< "     ";
            if(this->get_y() == 28)
            {
                gotoxy(x-1,28);
                cout<< "     ";
            }



        }

        void EraseOldAsteroidat(int x, int y){
            gotoxy(x,y);
            cout << "     " << endl;
        }

        //initial Erase function for single characters
        void Erase(){
            gotoxy(x,y);
            cout<<" ";
        }


        void Erase(int x, int y){
            gotoxy(x,y);
            cout<< " " << " " << " " << endl;
        }

        void Pause(){
            gotoxy(57,15);
            cout << "PAUSED" << endl;
            gotoxy(58,16);
            cout << "Press" << endl;
            gotoxy(58,17);
            cout << "space" << endl;
            gotoxy(56,18);
            cout << "to resume" << endl;
        }
        void Unpause(){
            gotoxy(57,15);
            cout << "      " << endl;
             gotoxy(56,16);
            cout << "         " << endl;
            gotoxy(56,17);
            cout << "         " << endl;
            gotoxy(56,18);
            cout << "         " << endl;
        }

        void DrawLaserText(){
            gotoxy(57,13);
            cout << "LASERS" << endl;
            gotoxy(57,14);
            cout << "INCOMING" << endl;
        }

        void  EraseLaserText(){
            gotoxy(57,13);
            cout << "      " << endl;
            gotoxy(57,14);
            cout << "        " << endl;
        }

        void DrawLaser(int i, int j,int& location, char aray[3] = "***"){
            if(j%3 == 2 || j%3 == 1) return;
            if(i == 0){
                location = rand() % 3 ;
                if(location == 0) {location = 50;}
                if(location == 1) {location = 60;}
                if(location == 2) {location = 70;}
            }


                if (i > 14) this->SetPoint(location,28);
                else this->SetPoint(location, 2*i+1);

                if (2*i < 27){
                    gotoxy(location-1,2*i);
                    cout<< aray << endl;
                    gotoxy(location-1,2*i+1);
                    cout << aray << endl;
                }


        }

        void EraseLaser(int i,int j,int location, char aray[3] = "   "){
            if(j%3 != 2) return;


            else{
                gotoxy(this->x-1,i);
            }
            cout<< aray << endl;

            if(i == 28)
            {
                gotoxy(location-1,28);
                cout<< "     ";
            }

        }
        void DrawScoreBoostat(int i,int& asteroid1location){
            int location;
            if(asteroid1location == 70) {location = 50;}
            if(asteroid1location == 60) {location = 70;}
            if(asteroid1location == 50) {location = 60;}
            if(i == 25) this->SetPoint(0,0);
            if(i >= 25) return;
            //setpt
            this->SetPoint(location,i+1);
            //gototxy
            gotoxy(location-3,i);
            gotoxy(location-3,i);
            cout << "|SCORE|" << endl;
            gotoxy(location-3,i+1);
            cout << "|BOOST|" << endl;

        }

        void ErasePowerupat(int i,int& asteroid1location){
            int location;

            if(asteroid1location == 70) {location = 50;}
            if(asteroid1location == 60) {location = 70;}
            if(asteroid1location == 50) {location = 60;}

            gotoxy(location-3,i-1);
            cout << "       " << endl;

        }

};

int main()
{
    //default high scores, need to be put up here to make sure they don't reset every time the game gets reset
    vector<int> scores = {60000,50000,40000,50,0};
    vector<string> names = {"Sally", "Leo","Rigby","Ben","Brandon"};


MAINMENU: //if the player dies, they have the option of ending the game or coming back here to the start screen
    /*
    Initialize all Point objects we need. Essentially, anything that needs to be outputted
    onto the screen will be a point class.
    */

    //1) The 4 dividers that will make up our 3 lanes
    Point farleft;
    Point middleleft;
    Point middleright;
    Point farright;

    //2) The asteroids and lasers that we have to avoid
    Point asteroid1;
    Point asteroid2;
    Point asteroid3;
    Point asteroid4;
    Point asteroid5;
    Point Laser1;
    Point Laser2;
    bool show = true;

    //3) a Counter for our score and a Warning Message that tells us when another Asteroid is coming
    Point score;
    Point Warning;


    //Powerups
    Point scoreboost;


    //4) Our Spaceship, or plane. call SetPoint to lock its position to the bottom of the screen as soon as the game starts
    Point plane;
    plane.SetPoint(60,25);

    //Start at Difficulty 1, at the slowest speed with the lowest score multiplier
    int difficulty = 1;
    int speed = 100;
    int scoremultiplier = 2;
    int laserround;

    //the characters we'll be using to read keyboard input and to restart the game
    char op;
    char restart;

    //The x-coordinates of the 3 lanes, and the staring location of each asteroid
    int rownumber1 = 50;
    int rownumber2 = 60;
    int rownumber3 = 70;
    int rownumber4 = 70;
    int rownumber5 = 70;
    int laserrow = 70;

    //delay of each asteroid, so they they come out at a seemingly random time
    int delaynumber1 = 0;
    int delaynumber2 = 6;
    int delaynumber3 = 12;
    int delaynumber4 = 18;
    int delaynumber5 = 24;

    //i keeps track of our y location on the screen and j counts how many times i has been run
    int i =0;
    int j = 0;

    //counts what level we're at
    int level = 1;
    //move the cursor away from our printed characters and into the corner
    setcursor(0,0);



    //Start Screen
    WelcomeScreen();
    while(getch() != ' '){
        }
    system("cls");
    Point pause;

    while(true){


        if(i == 29) {

            i = 0;
            j+=1;
            rownumber1 = plane.get_x();
            //cout<< j << endl;

            if (j >= 10) {
                difficulty = 2;
            }

            if (j >= 30) {
                difficulty = 3;
            }


            if (j >= 70) {
                difficulty = 4;
            }

            if (j >= 110) {
                difficulty = 5;
            }

            if(j>49){
            laserround = j%100;
            if (laserround == 50) show = false;
            if ( laserround > 50 && laserround < 53){
                Laser1.DrawLaserText();
            }
            if(laserround == 53) Laser1.EraseLaserText();
            if ( laserround >= 53 && laserround < 87) {
                difficulty = 6;
            }

            //move the lasers
            if (laserround == 87) {
                Laser1.SetPoint(0,0);
                Laser2.SetPoint(0,0);
                //level = 5;
            }

            if(laserround == 90){
                show=true;
            }
        }

        }


        // player's score gets increased by 2 everytime this loop is run, and can go up by 10000 if they get a score boost
        plane.score += scoremultiplier;
        plane.DrawPlaneScore();
        plane.DrawLevel(level);
        if(j%100 == 87 || j%100 == 20){
                scoreboost.DrawScoreBoostat(i,rownumber1);
                scoreboost.ErasePowerupat(i,rownumber1);
        }
        if(j%100 == 88 || j%100 == 21){
            scoreboost.hit = false;
        }

        /*
        check to see if its time to move
        onto the next difficulty
        */


        //Only 1 asteroid to avoid on difficulty 1
        if(difficulty == 1){
            asteroid1.DrawAsteroidat(i,delaynumber1,rownumber1,show);

        }

        //2 asteroids to avoid on difficulty 2
        else if (difficulty == 2){
            asteroid1.DrawAsteroidat(i,delaynumber1,rownumber1,show);
            asteroid2.DrawAsteroidat(i,delaynumber2,rownumber2,show);

        }
        //3 asteroids to avoid on difficulty 3
        else if (difficulty == 3){
            asteroid1.DrawAsteroidat(i,delaynumber1,rownumber1,show);
            asteroid2.DrawAsteroidat(i,delaynumber2,rownumber2,show);
            asteroid3.DrawAsteroidat(i,delaynumber3,rownumber3,show);

        }
        //4 asteroids to avoid on difficulty 4
        else if (difficulty == 4){
            asteroid1.DrawAsteroidat(i,delaynumber1,rownumber1,show);
            asteroid2.DrawAsteroidat(i,delaynumber2,rownumber2,show);
            asteroid3.DrawAsteroidat(i,delaynumber3,rownumber3,show);
            asteroid4.DrawAsteroidat(i,delaynumber4,rownumber4,show);
        }
        //5 asteroids to avoid on difficulty 5
        else if (difficulty == 5){
            asteroid1.DrawAsteroidat(i,delaynumber1,rownumber1,show);
            asteroid2.DrawAsteroidat(i,delaynumber2,rownumber2,show);
            asteroid3.DrawAsteroidat(i,delaynumber3,rownumber3,show);
            asteroid4.DrawAsteroidat(i,delaynumber4,rownumber4,show);
            asteroid5.DrawAsteroidat(i,delaynumber5,rownumber5,show);


        }
        //Laser Level
        else if (difficulty == 6){
            asteroid1.DrawAsteroidat(i,delaynumber1,rownumber1,show);
            asteroid2.DrawAsteroidat(i,delaynumber2,rownumber2,show);
            asteroid3.DrawAsteroidat(i,delaynumber3,rownumber3,show);
            asteroid4.DrawAsteroidat(i,delaynumber4,rownumber4,show);
            asteroid5.DrawAsteroidat(i,delaynumber5,rownumber5,show);
            Laser1.DrawLaser(i,j,rownumber5);
            Laser1.EraseLaser(i,j,rownumber5);
            Laser2.DrawLaser(i,j,rownumber1);
            Laser2.EraseLaser(i,j,rownumber1);

        }
        //set the location of our dividers, but don't print them yet
        farleft.SetPoint(45, i);
        middleleft.SetPoint(55, i);
        middleright.SetPoint(65, i);
        farright.SetPoint(75, i);
        // print our dividers and our spaceship
        farleft.Draw('|');
        middleleft.Draw('|');
        middleright.Draw('|');
        farright.Draw('|');
        plane.DrawPlane();


        //check if the plane collided with any of the asteroids or Lasers
        if(asteroid1.get_x() == plane.get_x() && (asteroid1.get_y() >= plane.get_y() && asteroid1.get_y() <= 29)){
            plane.hit = true;
        }
        if(asteroid2.get_x() == plane.get_x() && (asteroid2.get_y() >= plane.get_y() && asteroid2.get_y() <= 29)){
            plane.hit = true;
        }
        if(asteroid3.get_x() == plane.get_x() && (asteroid3.get_y() >= plane.get_y() && asteroid3.get_y() <= 29)){
            plane.hit = true;
        }
        if(asteroid4.get_x() == plane.get_x() && (asteroid4.get_y() >= plane.get_y() && asteroid4.get_y() <= 29)){
            plane.hit = true;
        }
        if(asteroid5.get_x() == plane.get_x() && (asteroid5.get_y() >= plane.get_y() && asteroid5.get_y() <= 29)){
            plane.hit = true;
        }
        if(Laser1.get_x() == plane.get_x() && (Laser1.get_y() >= plane.get_y() && Laser1.get_y() <= 29)){
            plane.hit = true;
        }
        if(Laser2.get_x() == plane.get_x() && (Laser2.get_y() >= plane.get_y() && Laser2.get_y() <= 29)){
            plane.hit = true;
        }
        if(scoreboost.get_x() == plane.get_x() && (scoreboost.get_y() >= plane.get_y() && scoreboost.get_y() <= 29) && scoreboost.hit == false){
            scoreboost.hit = true;
            plane.score += 10000;
        }

        if(plane.hit == true){
            plane.EraseOldPlane(plane.get_x(), 25);
            GameOver(scores, names, plane.score);
            //HighScores(scores, names, plane.score);
  CHECK:    restart = getch();
            if(restart == 'r'){
                goto MAINMENU;
            }
            else if(restart == ' ' ){
                break;
            }
            else goto CHECK;
        }
        // read in keyboard input and see if we have to move left(75) or right(77), or just stay where we are
        if(kbhit()){
            op = getch();
        }

        //left
        if((int)op == 75){
            if(plane.get_x() == 70) {
                plane.SetPoint(60,25);
                plane.EraseOldPlane(70,25);
            }
            else if(plane.get_x() == 60){
                plane.SetPoint(50,25);
                plane.EraseOldPlane(60,25);
            }
            else {plane.SetPoint(50,25);}
            op = 'l';
        }
        //right
        if((int)op == 77){
            if(plane.get_x() == 50){
                plane.SetPoint(60,25);
                plane.EraseOldPlane(50,25);
            }
            else if(plane.get_x() == 60){
                plane.SetPoint(70,25);
                plane.EraseOldPlane(60,25);
            }
            else {plane.SetPoint(70,25);}
            op = 'l';
        }

        if(op == ' '){
            pause.Pause();
            while(getch() != ' '){
            }
            pause.Unpause();
            op = 'l';
        }
        else {}

        //every time the original asteroid gets spawned at the top, increase the speed of the game
        if ( i == 0) speed -=3;
        //make sure that the game doesn't go too fast too soon and break itself
        if (speed < 65 && plane.score<5000) speed = 65;
        if (speed < 55 && plane.score<20000) speed = 55;
        if (speed < 45 && plane.score<50000) speed = 45;
        if (speed < 35 && plane.score<100000) speed = 35;
        if (speed < 25 && plane.score<200000) speed = 25;
        if(plane.score % 1000 == 0 || plane.score % 1000 == 500) {level += 1;}

        i += 1;
        Sleep(speed);
    }
    return 0;
}
