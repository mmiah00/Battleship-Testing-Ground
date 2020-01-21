#include "networking.h"
#include "battleship.c"
#include "msg.h"

struct gameBoard server_board;
struct gameBoard client_board;
struct gameBoard buffer_board;

int main(int argc, char **argv) {

  // Messages settings
  int end_game;
  status_message *s_msg;
  introducion_message *i_msg;
  attack_message *a_msg;

// The sockets
  int server_socket;

// commands and boards
  char commands[BUFFER_SIZE];
  client_board.player = 2;
  server_board.player = 1;


  if (argc == 2)
    server_socket = client_setup(argv[1]);
  else
    server_socket = client_setup( TEST_IP );

  printf ("Let's place your ships!\n\n");
  placingShips (server_board, 2, 1);
  printf("\nShips ready!\n");
  // ------------------------------------------------------------------------

  s_msg = malloc(sizeof(status_message));
  s_msg->type = 3;
  s_msg->response = 2;
  send(server_socket, s_msg, sizeof(status_message), 0);
  free(s_msg);


  // ------------------------------------------------------------------------
    // Game starts

    while(1)
    {
        // Server attacks -----------------------------------------------------

        show_map(m); //Our map (We have to put our print map)


        // Receive attack
        printf("Waiting for an attack\n");
        struct coordinate c;
        c = malloc (sizeof(struct coordinate));   //a_msg = malloc(sizeof(attack_message));
        recv(server_socket, c, sizeof(struct coordinate), 0);

        // Send response
        int status;
        status = malloc (sizeof (int));
        //s_msg = malloc(sizeof(status_message));
        //s_msg->type = 3;
        status = client_board.board[c.x][c.y];
        // if (client_board.board[c.x][c.y] == 1) // if hit
        // {
        //     if (check_map(m) == 0) // Check for win conditions here
        //     {
        //         s_msg->response = 3; // This means lost
        //         send(server_socket, s_msg, sizeof(status_message), 0);
        //         free(a_msg);
        //         free(s_msg);
        //         system("clear");
        //         PRINT_RED("\nYOU LOST!!!\n\n");
        //         close(server_socket);
        //         exit(1);
        //     }
        //     else
        //     {
        //         s_msg->response = 1; // This means hit
        //     }
        // }
        // else {
        //     s_msg->response = 0; // This means miss
        // }

        send(server_socket, status, sizeof(int), 0); //send to the server
        free (c); //free(a_msg);
        free (status); //free(s_msg);

        system("clear");

        // Your turn to attack ------------------------------------------------

        show_map(m); // print map


        printf("Your turn to attack\n");
        printf("x: ");
        scanf("%i", &x);
        printf("y: ");
        scanf("%i", &y);

        // Send attack (VIA COORS)
        a_msg = malloc(sizeof(attack_message));
        a_msg->type = 2;
        a_msg->x = x;
        a_msg->y = y;
        send(server_socket, a_msg, sizeof(attack_message), 0);
        free(a_msg);

        // Get response (CHECKS if won)
        s_msg = malloc(sizeof(status_message));
        recv(server_socket, s_msg, sizeof(status_message), 0);

        switch (s_msg->response)
        // This is where we find out if the coordinates hit a ship
        {
            case 0: // A miss so nothing happens
                server_map->map[y][x] = MISS;
                break;
            case 1: // A hit
                server_map->map[y][x] = HIT;
                break;
            case 3: //A win
                system("clear");
                printf("\nYOU WON!!!\n\n");
                close(server_socket);
                exit(1);
            default:
                break;
        }
        free(s_msg);

        system("clear");
    }

    close(server_socket);

    return 0;

}
