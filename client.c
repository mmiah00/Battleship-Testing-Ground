#include "networking.h"
#include "battleship.c"
#include "msg.h"

struct gameBoard server_board;
struct gameBoard client_board;
struct gameBoard buffer_board;

int main(int argc, char **argv) {

  // Messages settings
  int end_game;
  int * status //status_message *s_msg;
  introducion_message *i_msg;
  struct coordinate *cor;

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

  status = malloc (sizeof (int*));
  send(server_socket, status, sizeof(int *), 0);
  free(status);

  // s_msg = malloc(sizeof(status_message));
  // s_msg->type = 3;
  // s_msg->response = 2;
  // send(server_socket, s_msg, sizeof(status_message), 0);
  // free(s_msg);


  // ------------------------------------------------------------------------
    // Game starts

    while(1)
    {
        // Server attacks -----------------------------------------------------

        display("ally", client_board.player, client_board, server_board); //show_map(m); //Our map (We have to put our print map)


        // Receive attack
        printf("Waiting for an attack\n");
        cor = malloc (sizeof(struct coordinate));   //a_msg = malloc(sizeof(attack_message));
        recv(server_socket, cor, sizeof(struct coordinate), 0);

        // Send response

        status = malloc (sizeof (int*));
        //s_msg = malloc(sizeof(status_message));
        //s_msg->type = 3;
        status = &client_board.board[c.x][c.y];
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
        free (cor); //free(a_msg);
        free (status); //free(s_msg);

        system("clear");

        // Your turn to attack ------------------------------------------------

        display ("enemy", client_board.player, client_board, server_board); //show_map(m); // print map


        char input [1000];                                  // printf("x: ");
        printf("Your turn to attack\n");                    // scanf("%i", &x);
        printf ("Type in your coordinates: ");              // printf("y: ");
        fgets (input, sizeof (input), stdin);               // scanf("%i", &y);
        char ** cords = parse_args (input);

        // Send attack (VIA COORS)                          // Send attack (VIA COORS)
        cor = malloc(sizeof(coordinate));                   //a_msg = malloc(sizeof(attack_message));
        cor->x = atoi (cords[0]);                           //a_msg->type = 2;
        cor->y = atoi (cords[1]);                           //a_msg->x = x;
        send(client_socket, cor, sizeof(coordinate), 0);    //a_msg->y = y;
        free(cor);                                          //send(server_socket, a_msg, sizeof(attack_message), 0);
                                                            //free(a_msg);

        // Get response (CHECKS if won)
        status = malloc(sizeof(int*)); //s_msg = malloc(sizeof(status_message));
        recv(server_socket, status, sizeof(int*), 0);

        if (finished (&server_board, &client_board)){
          system (clear);
          exit (1);
        }
        // switch (s_msg->response)
        // // This is where we find out if the coordinates hit a ship
        // {
        //     case 0: // A miss so nothing happens
        //         server_map->map[y][x] = MISS;
        //         break;
        //     case 1: // A hit
        //         server_map->map[y][x] = HIT;
        //         break;
        //     case 3: //A win
        //         system("clear");
        //         printf("\nYOU WON!!!\n\n");
        //         close(server_socket);
        //         exit(1);
        //     default:
        //         break;
        // }

        free (status); //free(s_msg);

        system("clear");
    }

    close(server_socket);

    return 0;

}
