#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

// Define game constants
const int WIDTH = 30;
const int HEIGHT = 20;
const int INITIAL_LENGTH = 5;
const char SNAKE_BODY = 'O';
const char FOOD = '*';

// Define directions
const int DIR_UP = 1;
const int DIR_DOWN = 2;
const int DIR_LEFT = 3;
const int DIR_RIGHT = 4;

struct SnakeSegment {
	int x, y;
};

#include <vector>
#include <queue>

struct Point {
	int x, y;
};

struct Node {
	Point pos;
	int g; // Cost from start to current node
	int h; // Heuristic (estimated cost from current node to target)
	int f; // Total cost (f = g + h)

	bool operator>(const Node& other) const {
		return f > other.f;
	}
};

// Define directions (assuming 4 directions: up, down, left, right)
const int DX[] = {0, 0, -1, 1};
const int DY[] = {-1, 1, 0, 0};

// A* pathfinding function
int findPath(const Point& start, const Point& target, int width, int height) {
	std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;

	Node startNode{start, 0, abs(target.x - start.x) + abs(target.y - start.y), 0};
	openSet.push(startNode);

	while (!openSet.empty()) {
		Node currentNode = openSet.top();
		openSet.pop();

		Point currentPos = currentNode.pos;
		int g = currentNode.g;

		// Check if we've reached the target
		if (currentPos.x == target.x && currentPos.y == target.y) {
			// Determine the direction to move
			for (int dir = 0; dir < 4; ++dir) {
				int newX = start.x + DX[dir];
				int newY = start.y + DY[dir];
				if (newX == currentPos.x && newY == currentPos.y) {
					return dir + 1; // Return direction (1=up, 2=down, 3=left, 4=right)
				}
			}
		}

		visited[currentPos.y][currentPos.x] = true;

		// Expand to neighboring cells
		for (int dir = 0; dir < 4; ++dir) {
			int newX = currentPos.x + DX[dir];
			int newY = currentPos.y + DY[dir];

			// Check if the new position is within bounds and not visited
			if (newX >= 0 && newX < width && newY >= 0 && newY < height && !visited[newY][newX]) {
				// Calculate g, h, and f values
				int newG = g + 1; // Assuming a cost of 1 to move to a neighboring cell
				int newH = abs(target.x - newX) + abs(target.y - newY);
				int newF = newG + newH;

				openSet.push({{newX, newY}, newG, newH, newF});
			}
		}
	}

	return 0; // No valid path found
}

// Function to simulate an automated snake's movement (pathfinding)
void simulateAutomatedSnake(WINDOW* win, int borderSize, int foodX, int foodY) {
	int headX = WIDTH / 4; // Initial position for automated snake
	int headY = HEIGHT / 4;

	int direction = DIR_RIGHT;

	while (true) {
		// Calculate new head position based on pathfinding logic (simplified)
		if (headX < foodX) {
			direction = DIR_RIGHT;
			headX++;
		} else if (headX > foodX) {
			direction = DIR_LEFT;
			headX--;
		} else if (headY < foodY) {
			direction = DIR_DOWN;
			headY++;
		} else if (headY > foodY) {
			direction = DIR_UP;
			headY--;
		}
		/*direction = findPath({headX, headY}, {foodX, foodY}, WIDTH, HEIGHT);
		switch(direction){
			case DIR_LEFT:headX--;break;
			case DIR_RIGHT:headX++;break;
			case DIR_UP:headY--;break;
			case DIR_DOWN:headY++;break;
			default:headX++;break;
		}*/

		// Draw the automated snake
		mvaddch(headY + borderSize, headX + borderSize, SNAKE_BODY);
		wrefresh(win);

		// Simulate a delay for snake's movement
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		// Clear the automated snake's previous position
		mvaddch(headY + borderSize, headX + borderSize, ' ');

		// Check if the automated snake reaches the food
		if (headX == foodX && headY == foodY) {
			// Generate new food position
			do {
				foodX = std::rand() % WIDTH + borderSize;
				foodY = std::rand() % HEIGHT + borderSize;
			} while (mvinch(foodY, foodX) == SNAKE_BODY);
		}
	}
}

int main(int argc, char* argv[]) {
	// Check for the correct number of command line arguments
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <border_size>" << std::endl;
		return 1;
	}

	// Parse the command line argument for border size
	int borderSize = std::atoi(argv[1]);

	// Initialize ncurses
	initscr();
	noecho();
	cbreak();
	curs_set(0);
	keypad(stdscr, true);
	nodelay(stdscr, true);

	// Initialize random seed
	std::srand(std::time(nullptr));

	// Create the game window and draw the border
	WINDOW* win = newwin(HEIGHT + 2 * borderSize, WIDTH + 2 * borderSize, 0, 0);
	box(win, 0, 0);
	wrefresh(win);

	// Show the automated snake example
	mvprintw(HEIGHT / 2 - 1, WIDTH / 2 - 15, "Automated Snake Example:");
	mvprintw(HEIGHT / 2, WIDTH / 2 - 15, "Pathfinding to the food!");
	mvprintw(HEIGHT / 2 + 1, WIDTH / 2 - 25, "Click anywhere to start the game...");
	refresh();

	// Wait for user to click anywhere
	std::cout << "Press any key to continue" << std::endl;
	std::cin.get();

	// Clear the screen
	clear();

	// Initialize game variables
	int headX = WIDTH / 2;
	int headY = HEIGHT / 2;
	int score = 0;
	std::vector<SnakeSegment> snake;
	snake.push_back({headX, headY});
	int direction = KEY_RIGHT;

	// Generate initial food position
	int foodX, foodY;
	// Simulate the automated snake's movement
	std::thread automatedSnake(simulateAutomatedSnake, win, borderSize, foodX, foodY);
	do {
		foodX = std::rand() % WIDTH + borderSize;
		foodY = std::rand() % HEIGHT + borderSize;
	} while (mvinch(foodY, foodX) == SNAKE_BODY);

	// Game loop
	while (true) {
		// Get user input
		int ch = getch();

		// Handle arrow key input to control the snake direction
		if ((ch == KEY_UP && direction != KEY_DOWN) ||
			(ch == KEY_DOWN && direction != KEY_UP) ||
			(ch == KEY_LEFT && direction != KEY_RIGHT) ||
			(ch == KEY_RIGHT && direction != KEY_LEFT)) {
			direction = ch;
		}

		// Calculate new head position
		int newHeadX = snake[0].x;
		int newHeadY = snake[0].y;
		if (direction == KEY_UP) newHeadY--;
		else if (direction == KEY_DOWN) newHeadY++;
		else if (direction == KEY_LEFT) newHeadX--;
		else if (direction == KEY_RIGHT) newHeadX++;

		// Check for collision with the border
		if (newHeadX <= borderSize || newHeadX >= WIDTH + borderSize ||
			newHeadY <= borderSize || newHeadY >= HEIGHT + borderSize) {
			break; // Game over
		}

		// Check for collision with itself
		for (const auto& segment : snake) {
			if (newHeadX == segment.x && newHeadY == segment.y) {
				break; // Game over
			}
		}

		// Move the snake
		snake.insert(snake.begin(), {newHeadX, newHeadY});

		// Check if the snake eats the food
		if (newHeadX == foodX && newHeadY == foodY) {
			score++;
			// Generate new food position
			do {
				foodX = std::rand() % WIDTH + borderSize;
				foodY = std::rand() % HEIGHT + borderSize;
			} while (mvinch(foodY, foodX) == SNAKE_BODY);
		} else {
			// Remove the tail segment
			mvaddch(snake.back().y, snake.back().x, ' ');
			snake.pop_back();
		}

		// Draw the snake
		for (const auto& segment : snake) {
			mvaddch(segment.y, segment.x, SNAKE_BODY);
		}

		// Draw the food
		mvaddch(foodY, foodX, FOOD);

		// Update the score
		mvprintw(HEIGHT + borderSize + 1, borderSize, "Score: %d", score);
		refresh();

		// Delay for smooth movement
		usleep(100000);
	}

	// Game over message
	mvprintw(HEIGHT / 2, WIDTH / 2 - 10, "Game Over!");
	mvprintw(HEIGHT / 2 + 1, WIDTH / 2 - 10, "Your Score: %d", score);
	mvprintw(HEIGHT / 2 + 2, WIDTH / 2 - 15, "Press any key to restart or 'q' to quit");
	refresh();

	// Cleanup and exit if 'q' is pressed
	int restartKey = getch();
	if (restartKey == 'q' || restartKey == 'Q') {
		automatedSnake.join(); // Stop the automated snake thread
		endwin();
		return 0;
	}

	// Cleanup and restart the game
	automatedSnake.join(); // Stop the automated snake thread
	clear();
	refresh();
	usleep(500000); // Pause for half a second before restarting

	// Cleanup and exit
	endwin();
	return 0;
}