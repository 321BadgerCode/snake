# Snake Game

## Backstory
Snake is a classic arcade game that has stood the test of time. It originated in the 1970s as a simple black-and-white game on early computer terminals. Over the years, it has evolved into various versions, making its way onto mobile phones and computers. The objective of the game is straightforward: control a snake and guide it to eat food while avoiding collisions with walls and itself. As the snake consumes food, it grows longer, making the game increasingly challenging.

## About the Game
This Snake game is a modern implementation of the classic arcade game. It features a snake that moves around the screen, eating food to grow longer. The player controls the snake's movement using arrow keys. The game includes a customizable border, allowing players to adjust the difficulty by specifying the border's size.

## A* Pathfinding Algorithm
One of the standout features of this Snake game is the implementation of the A* (A star) pathfinding algorithm to move an automated snake to the food. A* is a widely-used algorithm in computer science and game development for finding the shortest path between two points. In our Snake game, the A* algorithm is used to efficiently guide the automated snake to the food without hitting itself or the border.

### How A* Works
1. **Initialization**: The algorithm starts with an initial node (snake's head) and a target node (food). It calculates the cost to reach the target from the initial node, taking into account both the distance traveled so far (g) and a heuristic estimate of the remaining distance (h).

2. **Exploration**: A* explores neighboring nodes, considering the total cost (f = g + h) to reach each neighboring node. It prioritizes nodes with lower f values.

3. **Path Reconstruction**: Once the target node is reached, A* reconstructs the path by backtracking from the target through nodes with the lowest f values. This path represents the optimal route from the snake's current position to the food.

## Controls
- Use the arrow keys to control the snake's direction: up, down, left, and right.
- The objective is to guide the snake to eat food and grow longer.
- Avoid collisions with the border and the snake's own body.
- Press 'q' to quit the game at any time.

## Customization
- You can specify the size of the border using command-line arguments when starting the game. Adjust the border size to increase or decrease the game's difficulty.

## Enjoy the Game!
This Snake game offers a blend of classic gameplay and modern features. Have fun playing, and see if you can beat the automated snake's impressive pathfinding skills! If you have any questions or feedback, please feel free to reach out.

**Happy gaming!**