#include <iostream>
#include "raylib.h"
#include <vector>

using namespace std;

struct Point {
	int x, y;

	Point(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	Point()
	{
		x = 0;
		y = 0;
	}
};


struct Node {
	Point pos;
	int data;
	Node(Point _pos, int _data)
	{
		pos = _pos;
		data = _data;
	}
	Node() { data = 0; }
};

struct Quad {
	Point topLeft;
	Point botRight;

	Node* n;

	Quad* topLeftTree;
	Quad* topRightTree;
	Quad* botLeftTree;
	Quad* botRightTree;

	Quad()
	{
		topLeft = Point(0, 0);
		botRight = Point(0, 0);
		n = NULL;
		topLeftTree = NULL;
		topRightTree = NULL;
		botLeftTree = NULL;
		botRightTree = NULL;
	}
	Quad(Point topL, Point botR)
	{
		n = NULL;
		topLeftTree = NULL;
		topRightTree = NULL;
		botLeftTree = NULL;
		botRightTree = NULL;
		topLeft = topL;
		botRight = botR;
	}
	void insert(Node*);
	bool inBoundary(Point);
};

void Quad::insert(Node* node) {
	if (node == NULL) {
		return;
	}
	if (!inBoundary(node->pos)) {
		return;
	}

	if (abs(topLeft.x - botRight.x) <= 1 && abs(topLeft.y - botRight.y) <= 1) {
		if (n == NULL) {
			n = node;
		}
		return;
	}

	if ((topLeft.x + botRight.x) / 2 >= node->pos.x) {
		if ((topLeft.y + botRight.y) / 2 >= node->pos.y) {
			if (topLeftTree == NULL) {
				topLeftTree = new Quad(
					Point(topLeft.x, topLeft.y),
					Point((topLeft.x + botRight.x) / 2,
						(topLeft.y + botRight.y) / 2));
			}
			topLeftTree->insert(node);
		}
		else {
			if (botLeftTree == NULL) {
				botLeftTree = new Quad(
					Point(topLeft.x,
						(topLeft.y + botRight.y) / 2),
					Point((topLeft.x + botRight.x) / 2,
						botRight.y));
			}
			botLeftTree->insert(node);
		}
	}
	else {
		if ((topLeft.y + botRight.y) / 2 >= node->pos.y	) {
			if (topRightTree == NULL) {
				topRightTree = new Quad(
					Point((topLeft.x + botRight.x) / 2,
						topLeft.y),
					Point(botRight.x,
						(topLeft.y + botRight.y) / 2));
			}
			topRightTree->insert(node);
		}
		else {
			if (botRightTree == NULL) {
				botRightTree = new Quad(
					Point((topLeft.x + botRight.x) / 2,
						(topLeft.y + botRight.y) / 2),
					Point(botRight.x, botRight.y));
			}
			botRightTree->insert(node);
		}

	}
}

bool Quad::inBoundary(Point p)
{
	return (p.x >= topLeft.x && p.x <= botRight.x
		&& p.y >= topLeft.y && p.y <= botRight.y);
}

void drawQuad(Quad* n) {

	if (n == NULL) return;

	if (n->n != NULL) {
		DrawCircle(n->n->pos.x, n->n->pos.y, 3, RED);
	}

	DrawLine(n->topLeft.x, n->topLeft.y, n->topLeft.x, n->botRight.y, WHITE);
	DrawLine(n->topLeft.x, n->topLeft.y, n->botRight.x, n->topLeft.y, WHITE);
	DrawLine(n->topLeft.x, n->botRight.y, n->botRight.x, n->botRight.y, WHITE);
	DrawLine(n->botRight.x, n->topLeft.y, n->botRight.x, n->botRight.y, WHITE);

	int midx = (n->topLeft.x + n->botRight.x) / 2;
	int midy = (n->topLeft.y + n->botRight.y) / 2;
	DrawLine(midx, n->topLeft.y, midx, n->botRight.y, WHITE);
	DrawLine(n->topLeft.x, midy, n->botRight.x, midy, WHITE);

	drawQuad(n->topLeftTree);
	drawQuad(n->topRightTree);
	drawQuad(n->botLeftTree);
	drawQuad(n->botRightTree);
}

int main() {


	Quad center(Point(0, 0), Point(800, 800));
	vector<Node> nodes;

	InitWindow(800, 800, "Quadtree");
	SetTargetFPS(60);  

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			nodes.emplace_back(Point(GetMouseX(), GetMouseY()), 0);
			center.insert(&nodes.back());
		}
		Quad* n = &center;
		drawQuad(n);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
