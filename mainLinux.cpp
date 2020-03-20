#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int M = 20;
const int N = 10;
const int MID = 4;
const int DN = 9;

int field[M][N] = {0};

struct Point
{int x,y;} a[4], b[4], c[4];

int figures[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};



int scoreInc[4] =
{
    40,
    100,
    300,
    1200
};

bool check()
{
   for (int i=0;i<4;i++)
      if (a[i].x<0 || a[i].x>=N || a[i].y>=M) return 0;
      else if (field[a[i].y][a[i].x]) return 0;

   return 1;
};


int main()
{
    srand(time(0));     

    RenderWindow window(VideoMode(320, 480), "Tetris - JTM");

    Texture t1,t2,t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/background.png");
    t3.loadFromFile("images/frame.png");

    Sprite s(t1), background(t2), frame(t3);

    int dx=0; 
	bool rotate=0; 
	int colorNum=1;
    float timer=0,delay=0.3;
    int scoreVal = 0;
	int nbLignes = 0;
	char score[12];
	int game = 1;

    Clock clock;
	
	// Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        return EXIT_FAILURE;
	
	// Init first piece
	int n=rand()%7;
	int next=rand()%7;
	int colorNext=1+next;
	colorNum=1+n;
	for (int i=0;i<4;i++)
	{
		a[i].x = (figures[n][i] % 2) + MID;
		a[i].y = figures[n][i] / 2;
		
		c[i].x = (figures[next][i] % 2) + MID + DN;
		c[i].y = figures[next][i] / 2;
	}

    while (window.isOpen())
    {
		while(game) {
			float time = clock.getElapsedTime().asSeconds();
			clock.restart();
			timer+=time;

			Event e;
			while (window.pollEvent(e))
			{
				if (e.type == Event::Closed)
					window.close();

				if (e.type == Event::KeyPressed)
				  if (e.key.code==Keyboard::Up) rotate=true;
				  else if (e.key.code==Keyboard::Left) dx=-1;
				  else if (e.key.code==Keyboard::Right) dx=1;
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) delay=0.05;

			//// <- Move -> ///
			for (int i=0;i<4;i++)  { b[i]=a[i]; a[i].x+=dx; }
			if (!check()) for (int i=0;i<4;i++) a[i]=b[i];

			//////Rotate//////
			if (rotate)
			  {
				Point p = a[1]; //center of rotation
				for (int i=0;i<4;i++)
				  {
					int x = a[i].y-p.y;
					int y = a[i].x-p.x;
					a[i].x = p.x - x;
					a[i].y = p.y + y;
				   }
				   if (!check()) for (int i=0;i<4;i++) a[i]=b[i];
			  }

			///////Tick//////
			if (timer>delay)
			  {
				for (int i=0;i<4;i++) { b[i]=a[i]; a[i].y+=1; }

				if (!check())
				{
				 for (int i=0;i<4;i++) field[b[i].y][b[i].x]=colorNum;

				 int n=next;
				 next = rand()%7;
				 colorNum=1+n;
			     colorNext=1+next;
				 for (int i=0;i<4;i++)
				   {
					   
					a[i].x = c[i].x - DN;
					a[i].y = c[i].y;
					
					c[i].x = (figures[next][i] % 2) + MID + DN;
					c[i].y = figures[next][i] / 2;
				   }
				   if (!check()) {
					   break;
				   }
				}

				 timer=0;
			  }

			///////check lines//////////
			int k=M-1;
			nbLignes = 0;
			for (int i=M-1;i>0;i--)
			{
				int count=0;
				for (int j=0;j<N;j++)
				{
					if (field[i][j]) count++;
					field[k][j]=field[i][j];
				}
				if (count<N) {
					k--;
				} else {
					nbLignes++;
				}
			}
			
			if(nbLignes > 0) {
				scoreVal += scoreInc[nbLignes-1];
				nbLignes = 0;
			}

			dx=0; rotate=0; delay=0.3;

			/////////draw//////////
			
			sprintf(score, "%d", scoreVal);
			sf::Text text(score, font, 50);
			text.setPosition(30, 400);
			
			window.clear(Color::White);    
			window.draw(background);
				  
			for (int i=0;i<M;i++)
			 for (int j=0;j<N;j++)
			   {
				 if (field[i][j]==0) continue;
				 s.setTextureRect(IntRect(field[i][j]*18,0,18,18));
				 s.setPosition(j*18,i*18);
				 s.move(28,31); //offset
				 window.draw(s);
			   }

			for (int i=0;i<4;i++)
			  {
				s.setTextureRect(IntRect(colorNum*18,0,18,18));
				s.setPosition(a[i].x*18,a[i].y*18);
				s.move(28,31); //offset
				window.draw(s);
			  }
			  
			// Draw next
			for (int i=0;i<4;i++)
			  {
				s.setTextureRect(IntRect(colorNext*18,0,18,18));
				s.setPosition(c[i].x*18,c[i].y*18);
				s.move(28,31); //offset
				window.draw(s);
			  }

			window.draw(frame);
			window.draw(text);
			window.display();
		}
    }

    return 0;
}
