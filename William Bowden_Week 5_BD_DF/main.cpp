#include <iostream>
#include "glut.h"
#include <ctime>
#include <stack>
#include <queue>
#include <Windows.h>
#include "vec2.h"
#include "drawCircle.h"

using namespace std;

class Node;

const int colors[] = { 0x0000FF, 0x1E90FF, 0x00BFFF, 0x87CEFA, 0xffffff, 0xffffff, 0x87CEFA, 0x00BFFF, 0x1E90FF, 0x0000FF };
const int STD_COLOR = 0;

struct Edge
{
	Node * from, *to;
	float cost;
	Edge( Node * start, Node * end );

	void draw();
};

class Node
{
public:
	vector<Edge> edges;
	char id;
	COORD position;
	int mark;

	bool AddNeighbor( Node * n )
	{
		for (int i = 0; i < (int)edges.size(); ++i)
		{
			if ( edges[i].to == n )	{ return false; }
		}
		edges.push_back( Edge( this, n ) );
		return true;
	}
	int getNeighborCount() { return edges.size(); }

	Node * getNeighbor( int index ){ return edges[index].to; }

	Node( char id, int x, int y ) : id( id ), mark( 0 )
	{
		position.X = x;
		position.Y = y;
	}

	void draw()
	{
		vec2 pos(position.X, position.Y);

		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < CIRCLE_POINTS; ++i)
		{
			vec2 p = pos + circle[i];
			glVertex2f(p.x, p.y);
		}

		glEnd();

		for (int i = 0; i < (signed)edges.size(); ++i)
		{
			edges[i].draw();
		}
		
	}

};

Edge::Edge(Node* start, Node* end) : from( start ), to( end )
{
	float dx = (float)( start->position.X - end->position.X );
	float dy = (float)( start->position.Y - end->position.Y );
	cost = sqrt( (float)( dx*dx + dy*dy ) );
		
}
void Edge::draw()
{
	vec2 start = vec2(from->position.X, from->position.Y);
	vec2 end = vec2(to->position.X, to->position.Y);
	glBegin(GL_LINES);
	glVertex2f(start.x, start.y);
	glVertex2f(end.x, end.y);
	glEnd();

}

void cpos(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), c );
}

void ccolor(int color)
{
	glColor3ubv((GLubyte*)&color);
}

void drawNode( Node* n )
{
	if (!n)
	{
		cout << "Hey you jerk, you are asking me to print nothing!" << endl;
	}
	cpos( n->position.X, n->position.Y );
	putchar( n->id );
}

Node * getNodeNamed( vector<Node> & nodes, char id )
{
	for (int i = 0; i < (int)nodes.size(); ++i)
	{
		if ( nodes[i].id == id ) { return &nodes[i]; }
	}
	return 0;
}

float distanceBetween( COORD position, Node* n )
{
	float dx = (float)(position.X - n->position.X);
	float dy = (float)(position.Y - n->position.Y);
	return sqrt( dx*dx + dy*dy );
}

vector<Node*> nodesWithin( COORD location, float maxDist, bool atLeast1Neighbor, vector<Node> & list )
{
	vector<Node*> out;
	float closest = -1;
	Node * closestNode = 0;
	for (int i = 0; i < (int)list.size(); ++i)
	{
		float dist = distanceBetween( location, &list[i] );
		if ( dist == 0 ) { continue; }
		if ( closest == -1 || dist < closest )
		{
			closest = dist;
			closestNode = &list[i];
		}
		if ( dist < maxDist )
		{
			out.push_back(&list[i]);
		}
	}
	if ( atLeast1Neighbor && out.size() == 0 && closestNode )
	{
		out.push_back( closestNode );
	}
	return out;
}

vector<Node> generateRandomGraph( COORD min, COORD max, int count, float maxNdist, bool atLeast1Neighbor ) 
{
	vector<Node> list;
	int dx = max.X - min.X;
	int dy = max.Y - min.Y;

	for ( int i = 0; i < count; ++i ) 
	{
		list.push_back( Node( ( i % 26 ) + 'a', min.X + rand() % dx, min.Y + rand() % dy ) );
	}
	for ( int i = 0; i < (signed)list.size(); ++i ) 
	{
		vector<Node*> neighbors = nodesWithin( list[i].position, maxNdist, atLeast1Neighbor, list );
		for ( int a = 0; a < (int)neighbors.size(); ++a ) 
		{
			Node * n = neighbors[a];
			if ( !n ) 
			{
				cout << "Y U no give good node?!" << endl;
			}
			list[i].AddNeighbor( n );
		}
		if (neighbors.size() == 1)
		{
			neighbors[0]->AddNeighbor(&list[i]);
		}
	}
	return list;
}


//1  procedure DFS-iterative(G,v):
void DFS(Node * v, std::vector<Node*> & a_output_dfs) {
	//2      let S be a stack
	std::stack<Node*> S;
	//3      S.push(v)
	S.push(v);
	//4      while S is not empty
	while (!S.empty()) {
		//5            v <-- S.pop() 
		v = S.top();
		S.pop();
		//6            if v is not labeled as discovered:
		if (v->mark == 0) {
			//7                label v as discovered
			v->mark = 1;
			a_output_dfs.push_back(v);
			//8                for all edges from v to w in G.adjacentEdges(v) do
			for (int i = 0; i < v->getNeighborCount(); ++i) {
				Node * w = v->getNeighbor(i);
				//9                    S.push(w)
				S.push(w);
			}
		}
	}
}

//1  procedure BFS(G,v) is
void BFS(Node * v, std::vector<Node*> & a_output_dfs) {
	std::queue<Node*> Q;				//2      create a queue Q
	// using Node::mark					//3      create a vector set V
	Q.push(v);							//4      enqueue v onto Q
	a_output_dfs.push_back(v);
	v->mark = 1;						//5      add v to V
	while (!Q.empty()) {					//6      while Q is not empty loop
		Node * t = Q.front();			//7         t <-- Q.dequeue()
		Q.pop();
		//8         if t is what we are looking for then
		//9            return t
		//10        end if
		//11        for all edges e in G.adjacentEdges(t) loop
		for (int i = 0; i < t->getNeighborCount(); ++i) {
			Node * u = t->getNeighbor(i);//12           u <-- G.adjacentVertex(t,e)
			if (u->mark == 0) {			//13           if u is not in V then
				u->mark = 1;			//14               add u to V
				a_output_dfs.push_back(u);
				Q.push(u);				//15               enqueue u onto Q
			}							//16           end if
		}								//17        end loop
	}									//18     end loop
}										//20 end BFS

void clearMarks(std::vector<Node> & graph) 
{
	for (int i = 0; i < (signed)graph.size(); ++i) {
		graph[i].mark = 0;
	}
}

class Game
{

public:

	COORD min, max;
	vector<Node> nodes;
	bool running;
	char userInput;
	Node * cursor;
	float distanceTraveled;
	static const char NEIGHBOR_LABEL_START = 'a';
	bool drawSearch;
	bool dfs, bfs;
	int animationIndex = 0;
	

	Game()
	{
		cout << "How many Nodes?: " << endl;
		int Input;
		cin >> Input;

		dfs = true;
		bfs = true;

		min.X = 2; 
		max.X = 70;
		min.Y = 2;
		max.Y = 20;
		srand(time(NULL));
		
		nodes = generateRandomGraph(min, max, Input, 18, true);
		running = true;
		cursor = &nodes[0];
	}

	vector<Node*> listOfNodesInSearchOrder;

	void update(int a_value)
	{

			
			clearMarks(nodes);
			listOfNodesInSearchOrder.clear();
			if (dfs) 
			{ DFS(cursor, listOfNodesInSearchOrder); }

			if (bfs)
			{ BFS(cursor, listOfNodesInSearchOrder); }

	}

	void draw()
	{
		if (drawSearch)
		{
				ccolor(STD_COLOR); // draw in white to start
			for (int i = 0; i < (signed)listOfNodesInSearchOrder.size(); ++i)
			{

				if (i >= (signed)animationIndex - 5 && i < (signed)animationIndex + 5)
				{
					int whichColor = i - animationIndex;
					ccolor(colors[whichColor + 5]);
					
				}
				else
				{
					ccolor(STD_COLOR);
				}
				listOfNodesInSearchOrder[i]->draw();
			}
			animationIndex++;
			if (animationIndex >= (signed)listOfNodesInSearchOrder.size())
			animationIndex = 0;
			Sleep(100);
		}
		else
		{
			ccolor(STD_COLOR);
			for (int i = 0; i < (int)nodes.size(); ++i)
			{
				nodes[i].draw();
			}
		}
	}

};

Game game;

void display()
{
	glClearColor(0, 1, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3ub(0, 0, 0);
	glBegin(GL_LINES);

	glEnd();
	game.draw();
	glFlush();
	glutSwapBuffers();
}

void keyboard(unsigned char k, int x, int y)
{
	game.drawSearch = !game.drawSearch;

	if (k == 'd')
	{
		game.bfs = false;
		game.dfs = true;
	}
	if (k == 'b')
	{
		game.dfs = false;
		game.bfs = true;
	}
}

time_t now = clock(), then, passed;
void gameUpdate(int value)
{
	then = now;
	now = clock();
	passed = now - then;
	game.update((int)passed);
	glutTimerFunc(value, gameUpdate, value);
	glutPostRedisplay();
}

void reshape(int a_width, int a_height)
{
	glLoadIdentity();
	gluOrtho2D(-1, 80, -1, 20);
	glutPostRedisplay();
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(640, 480);
	glViewport(0, 0, (int)640, (int)480);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("data structures");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	const int TARGETFPS = 40, TARGET_DELAY = 1000 / TARGETFPS;
	glutTimerFunc(0, gameUpdate, TARGET_DELAY);
	glutMainLoop();
	return 0;
}