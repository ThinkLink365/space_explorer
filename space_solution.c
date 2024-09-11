    //R00230462 - Liam Ó Dubhgáin
    #include "space_explorer.h"
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include <stdbool.h>
    #define MAX_SIZE 300

    typedef struct{
        int num_planets_visited; // variable to keep track of the number of planets visited
        double* candidate_for_shortest; // array to store each planets distance in a cycle
        unsigned int* planets_to_visit; // array of the planets we plan to visit
        unsigned int* planets_visited; // array to store the planets that have been visited
        unsigned int *not_visited; // array of the planets we have not visited
        int increment; // variable to increment our cycle
        unsigned int home_planet_id; // the id of the planet we start at in a cycle
        int current_num_connections; // the current number of connections in our cycle
        double shortest_distance_to_mixer; // the shorest distance to the mixer
        double second_shortest_distance_to_mixer; // the second shorest distance to the mixer (for backtracking)
        unsigned int second_shortest_planet_id; // the second shortest planet id
        int ship_stuck; // variable to count how many jumps the ship has been stuck
        double home_planet_distance; // the distance from the mixer of the home planet in the cycle
    } shipInfo;

    // function to calculate which planets we are yet to visit. this is done by taking the planets we have visited,
    // planets we plan to visit and checking which planets we plan to visit have been visited already and putting the ones we havent been to into a new arary called not_visited.
    unsigned int *planets_yet_to_visit(unsigned int *planets_visited, unsigned int *planets_to_visit, int num_planets_visited, int num_planets_to_visit, shipInfo *ship)
    {
        int count = 0;
        //first for loop takes the first planet of planets to visit and sets visted to false
        for (int i = 0; i < num_planets_to_visit; i++)
        {
            bool visited = false;
            // second for loop compares the the planet with the planets that have been visited and of there is a match then visited is set to true
            for (int j = 0; j < num_planets_visited; j++)
            {
                if (planets_to_visit[i] == planets_visited[j])
                {
                    visited = true;
                }
            }
            // otherwise we update the not visited array with that planet and increment count
            if (!visited)
            {
                ship->not_visited[count] = planets_to_visit[i];
                count++;
            }
        }
        // we update the current number of connections we have to visit
        ship->current_num_connections = count;

        return ship->not_visited;
    }

    // function to check if we are stuck so we can take action (backtrack or random hop)
    bool ship_stuck(shipInfo *ship) {
        // check if there are no more planets left to visit
        bool no_planets_left = (ship->current_num_connections == 0);
        return no_planets_left;
    }

    // the main space_hop function
    ShipAction space_hop(unsigned int crt_planet,
                        unsigned int *connections,
                        int num_connections,
                        double distance_from_mixer,
                        void *ship_state)
    {
        // start by intialising our structures
        shipInfo *ship;
        ShipAction next_jump;

        // if the shipstate is null then its the first time calling the function so we need to initalise our variables in ship state and allocate memory
        if (ship_state == NULL)
        {
            ship = malloc(sizeof(shipInfo));
            ship->candidate_for_shortest = malloc(MAX_SIZE * sizeof(double));
            ship->planets_to_visit = malloc(MAX_SIZE * sizeof(unsigned int)); 
            ship->planets_visited = malloc(MAX_SIZE * sizeof(int));
            ship->not_visited = malloc(MAX_SIZE * sizeof(unsigned int));
            ship->increment = 0;
            ship->home_planet_id = crt_planet;
            ship->current_num_connections = 0;
            ship->shortest_distance_to_mixer = 100.00; // set to a high value for calculating the shortest distance
            ship->second_shortest_distance_to_mixer = 100.00; // set to a high value for calculating the shortest distance
            ship->second_shortest_planet_id = 0; 
            ship->ship_stuck = 0 ;
            ship->home_planet_distance = 0.00;
        }   
        // otherwise we just need to keep updating our shipstate each call 
        else{
            ship = ship_state;
        }
        
        // while the distance form the mixer is greater than or equal to 3 we random hop to try and get closer
        while (distance_from_mixer >= 3)
        {
            next_jump.next_planet = RAND_PLANET;
            next_jump.ship_state = ship;
            return next_jump;
        }

        printf("\n");
        printf("Current planet: %d\n", crt_planet);
        printf("Number of connections: %d\n", num_connections);
        printf("Connections: ");
        for (int i = 0; i < num_connections; i++)
        {
            printf("%d ", connections[i]);
        }
        printf("\n");
        printf("Distance from the mixer: %f\n", distance_from_mixer);
        printf("Press Enter to start jumping\n");
        // while (getchar() != '\n'); // comment out this line to run all at once
        
        // if the increment is 0 then we are starting a new cycle of checking the planets around us in an attempt to find the shortest diatance.
        if (ship->increment == 0)
        {   
            // we copy the connections into planets to visit so we can keep the connections we want to visit when hooping to nearby planets 
            memcpy(ship->planets_to_visit, connections, num_connections * sizeof(unsigned int));
            // update the current number of connections in shipstate
            ship->current_num_connections = num_connections;
            // call the planets yet to visit function to see what planets we have visited already
            planets_yet_to_visit(ship->planets_visited,ship->planets_to_visit,ship->num_planets_visited,ship->current_num_connections,ship);
            // update the home planet distance in shipstate
            ship->home_planet_distance = distance_from_mixer;
        }
        // if the not_visited is null then we have to visit all the planets
        if (ship->not_visited == NULL)
        {
            // jump to the next planet
            next_jump.next_planet = ship->planets_to_visit[ship->increment];
            // store the planet we just visited in planets visited
            ship->planets_visited[ship->num_planets_visited] = crt_planet;
        }
        // otherwise we have to check the not visited array to check which planet to visit
        else{
            // jump to the next planet
            next_jump.next_planet = ship->not_visited[ship->increment];
            // store the planet we just visited in planets visited
            ship->planets_visited[ship->num_planets_visited] = crt_planet;
        }
        // if the increment is greater than 0 it means we have moved to the first planet we want to check
        if (ship->increment > 0)
        {    
            // store the distance for that planet in candidate ( -1 so we can store the distance with the correct planet index)
            ship->candidate_for_shortest[ship->increment - 1] = distance_from_mixer;
        }   
        printf("Planets visited: ");
        for (int i = 0; i <= ship->num_planets_visited; i++) {
            printf("%d ", ship->planets_visited[i]);
        }
        printf("\n");

        printf("Planets to visit: ");
        for (int i = 0; i < num_connections; i++) {
            printf("%d ", ship->planets_to_visit[i]);
        }
        printf("\n");

        printf("Candidate for shortest: ");
        for (int i = 0; i < num_connections; i++) {
            printf("%f ", ship->candidate_for_shortest[i]);
        }
        printf("\n");

        printf("\n");

        // increment our variables to continue the cycle
        ship->increment++;
        ship->num_planets_visited++;
            
        printf("Increment: %d\n", ship->increment);
            
        printf("Number of planets visited: %d\n", ship->num_planets_visited);

        // our logic to determine the closest planet and when to calculate it
        // if the increment is greater then our current number of connections, we've explorered all the planets for this cycle
        if (ship->increment > ship->current_num_connections)
        {
            // iterate through the candidates for the shortest
            for (int i = 0; i < ship->current_num_connections; i++)
            {
                // if the ship state is null then we must iterate through the planets_to_visit array
                if (ship->not_visited == NULL)
                {   
                    // if the currernt candidate for the shortest is less then this shortest distance and is is less then the home planet distance 
                    if (ship->candidate_for_shortest[i] < ship->shortest_distance_to_mixer && ship->candidate_for_shortest[i] < ship->home_planet_distance)
                    {   
                        // update the second shortest to be the previous shortest
                        ship->second_shortest_distance_to_mixer = ship->shortest_distance_to_mixer;
                        // update the second shortest planet to be the previous planet
                        ship->second_shortest_planet_id = ship->home_planet_id;
                        // update the shortest distance
                        ship->shortest_distance_to_mixer = ship->candidate_for_shortest[i];
                        // update the closet planet
                        ship->home_planet_id = ship->planets_to_visit[i];
                    }
                }
                // otherwise we iterate through planets not visited
                else{
                    if (ship->candidate_for_shortest[i] < ship->shortest_distance_to_mixer && ship->candidate_for_shortest[i] < ship->home_planet_distance)
                    {
                        // update the second shortest to be the previous shortest
                        ship->second_shortest_distance_to_mixer = ship->shortest_distance_to_mixer;
                        // update the second shortest planet to be the previous planet
                        ship->second_shortest_planet_id = ship->home_planet_id;
                        // update the shortest distance
                        ship->shortest_distance_to_mixer = ship->candidate_for_shortest[i];
                        // update the closet planet
                        ship->home_planet_id = ship->not_visited[i];
                    }
                }
            }
            // reset the variables for the next cycle
            ship->increment = 0;
            ship->shortest_distance_to_mixer = 100.00;
            // jump to the closest planet
            next_jump.next_planet = ship->home_planet_id;
        }
        
        printf("Home planet: %d\n", ship->home_planet_id);
        printf("Second shortest planet: %d\n", ship->second_shortest_planet_id);

        // check if the ship is stuck
        if (ship_stuck(ship)) {
            printf("\n");
            printf("Ship is stuck, backtracking");
            printf("\n");
            // jump to the second shortest planet
            next_jump.next_planet = ship->second_shortest_planet_id;
            // increment the ship_stuck counter
            ship->ship_stuck++;
            // if the ship gets stuck more than once then jump to a random planet
            if (ship->ship_stuck > 1)
            {
                next_jump.next_planet = RAND_PLANET;
                ship->ship_stuck = 0;
                printf("Jumped to a random planet \n");
            }
        }
        //update the ship state
        next_jump.ship_state = ship;
        return next_jump;
    }