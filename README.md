# Space Explorer - Planet Hopping Game

## How to Compile and Run:
Navigate to the directory containing the game files.
Compile the game using the following command:
## gcc space_explorer.c space_solution.c -lm -o space_explorer
Run the game with the executable:
## On Windows:

space_explorer.exe
## On Linux/macOS:

./space_explorer
## Testing with Different Seeds:
You can modify the randomness by changing the value of the seed in the code:

else {
    seed = 12;  // Default seed if not provided
}
## Note:
Some seeds might make the game impossible to complete without lucky random hops.
Randomization behavior may vary slightly between different operating systems.
