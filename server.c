#include "networking.h"
#include "msg.h"

struct gameBoard server_board;
struct gameBoard client_board;
struct gameBoard buffer_board;

int main() {
  // Messages settings
  int end_game;
  int * status //status_message *s_msg;
  introducion_message *i_msg;
  struct coordinate *cor;

  // The sockets
  int listen_socket;
  int client_socket;
  server_board.player = 1;
  client_board.player = 2;// copy to this


  //set of file descriptors to read from
  fd_set read_fds;

  listen_socket = server_setup();

  printf ("Let's place your ships!\n\n");
  placingShips (server_board, 2, 1);
  printf("\nShips ready!\n");
  // ------------------------------------------------------------------------

  printf ("Waiting for other player... \n");

  status = malloc(sizeof(int*));

  recv(client_socket, status, sizeof(int), 0);

  // if (status ->response != 2)
  // {
  //     exit(1);
  // }
  // free(s_msg);

  system("clear");
  // ------------------------------------------------------------------------
  // Game starts
  while (1) {

    //select() modifies read_fds
    //we must reset it at each iteration
    FD_ZERO(&read_fds); //0 out fd set
    FD_SET(STDIN_FILENO, &read_fds); //add stdin to fd set
    FD_SET(listen_socket, &read_fds); //add socket to fd set

    //select will block until either fd is ready
    select(listen_socket + 1, &read_fds, NULL, NULL, NULL);

    //if listen_socket triggered select
    if (FD_ISSET(listen_socket, &read_fds)) {
     client_socket = server_connect(listen_socket);

    }//end listen_socket select

    // ------------------------------------------------------------------------
    // Game starts
    while(1) {
    // Server attacks -----------------------------------------------------

    display ("ally", server_board.player, server_board, client_board);
    //display_my_board(server_board); ///Our map (We have to put our print map)

    char input [1000];                          // printf("x: ");
    printf("Your turn to attack\n");            // scanf("%i", &x);
    printf ("Type in your coordinates: ");      // printf("y: ");
    fgets (input, sizeof (input), stdin);       // scanf("%i", &y);
    char ** cords = parse_args (input);

    // Send attack (VIA COORS)
    cor = malloc(sizeof(coordinate));
    cor->x = atoi (cords[0]);
    cor->y = atoi (cords[1]);
    send(client_socket, cor, sizeof(coordinate), 0);
    free(cor);

    // Get response (CHECKS if won)
    status = malloc (sizeof (int));
    //s_msg = malloc(sizeof(status_message));
    recv(client_socket, status, sizeof(status), 0);

    client_board.board[cor->x][cor->y] = &status;

    if (finished (&server_board, &client_board)){
      system (clear);
      exit (1);
    }
    // switch (s_msg->response)
    // // This is where we find out if the coordinates hit a ship
    // {
    //     case 0: // A miss so nothing happens
    //         client_map->map[y][x] = MISS; //Change the status
    //         break;
    //     case 1: // A hit
    //         client_map->map[y][x] = HIT; //Change the status
    //         break;
    //     case 3: //A win
    //         system("clear");
    //         printf("\nYOU WON!!!\n\n");
    //         close(server_socket);
    //         close(client_socket);
    //         exit(1);
    //     default:
    //         break;
    // }

    free (status); //free(s_msg);

    system("clear");

    // Client attacks -----------------------------------------------------

    display ("ally", server_board.player, server_board, client_board); //show_map(m); // show my map

    // Receive attack
    printf("Waiting for an attack\n");
    cor = malloc(sizeof(cor));
    recv(client_socket, cor, sizeof(cor), 0);

    // Send response
    status = malloc (sizeof (int)); //s_msg = malloc(sizeof(status_message));
    //s_msg->type = 3;

    status = &server_board.board[cor->x][cor->y];
    // if (attack_ship(m, cor->x, cor->y) == 1) // if hit
    // {
    //     if (check_map(m) == 0) // Check for win conditions here
    //     {
    //         s_msg->response = 3; // This means lost
    //         send(client_socket, s_msg, sizeof(status_message), 0);
    //         free(cor);
    //         free(s_msg);
    //         system("clear");
    //         printf("\nYOU LOST!!!\n\n");
    //         close(server_socket);
    //         close(client_socket);
    //         exit(1);
    //     }
    //     else // It was just a hit
    //     {
    //         s_msg->response = 1;
    //     }
    // }
    // else { // This means it was a miss
    //     s_msg->response = 0;
    // }
    send(client_socket, status, sizeof(int*), 0);
    free(cor);
    free(status);
    system("clear");
}
}
}
