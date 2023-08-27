//Badger
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <cstdlib>
#include <ctime>

// Define game constants
const int WIDTH = 30;
const int HEIGHT = 20;
const int INITIAL_LENGTH = 5;
const char SNAKE_BODY = 'O';
const char FOOD = '*';
const char BORDER = '#';

// Define snake data structure
struct SnakeSegment {
	int x, y;
};

int main(int argc, char* argv[]) {
	// Check for the correct number of command line arguments
	int borderSize=5;
	if (argc > 2 || (argc == 2 && argv[1][0]+argv[1][1] == '-'+'h')) {
		std::cerr << "Usage: " << argv[0] << " <border_size(default is 5)>" << std::endl;
		return 1;
	}
	else if(argc == 2){
		// Parse the command line argument for border size
		int borderSize = std::atoi(argv[1]);
	}

	// Initialize ncurses
	initscr();
	noecho();
	cbreak();
	curs_set(0);
	keypad(stdscr, true);
	nodelay(stdscr, true);

	// Initialize random seed
	std::srand(std::time(nullptr));

	// Create the game window
	WINDOW* win = newwin(HEIGHT + 2 * borderSize, WIDTH + 2 * borderSize, 0, 0);
	box(win, 0, 0);
	wrefresh(win);

	// Initialize game variables
	int headX = WIDTH / 2;
	int headY = HEIGHT / 2;
	int score = 0;
	std::vector<SnakeSegment> snake;
	snake.push_back({headX, headY});
	int direction = KEY_RIGHT;

	// Generate initial food position
	int foodX, foodY;
	do {
		foodX = std::rand() % (((WIDTH + borderSize)-1)-borderSize)+borderSize;
		foodY = std::rand() % (((HEIGHT + borderSize)-1)-borderSize)+borderSize;
	} while (mvinch(foodY, foodX) == SNAKE_BODY);

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
		else if(ch == 'q'){
			break; // Game over
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
				std::cout << "\n\nYou just ate YOURSELF?!?" << std::endl;
				std::cout << "You know your supposed to eat the food, right?" << std::endl;
				std::cout << "Talk about getting hungry..." << std::endl;
				std::cout << "Final Score: " << score << std::endl;
				exit(0); // Game over
			}
		}

		// Move the snake
		snake.insert(snake.begin(), {newHeadX, newHeadY});

		// Check if the snake eats the food
		if (newHeadX == foodX && newHeadY == foodY) {
			score++;
			// Generate new food position
			do {
				foodX = std::rand() % (((WIDTH + borderSize)-1)-borderSize)+borderSize;
				foodY = std::rand() % (((HEIGHT + borderSize)-1)-borderSize)+borderSize;
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

		for(int a=borderSize;a<WIDTH+borderSize;a++){
			mvaddch(borderSize, a, BORDER);
			mvaddch(HEIGHT+borderSize, a, BORDER);
		}
		for(int a=borderSize;a<HEIGHT+borderSize;a++){
			mvaddch(a, borderSize, BORDER);
			mvaddch(a, WIDTH+borderSize, BORDER);
		}

		// Draw the food
		mvaddch(foodY, foodX, FOOD);

		// Update the score
		mvprintw(HEIGHT + borderSize + 1, borderSize, "Score: %d", score);
		refresh();

		// Delay for smooth movement
		usleep(100000);
	}
	refresh();

	// Cleanup and exit
	endwin();

	// Game over message
	std::cout << "Game over!" << std::endl;
	std::cout << "Final Score: " << score << std::endl;

	return 0;
}
// FIXME: bottom right of border is missing '#'.
// FIXME: food sometimes spawns out of bounds(on border).
// FIXME: printing game over doesn't format properly.
// FIXME: automated snake b4 game starts.
// TODO: color gradient on snake's segments.