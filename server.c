#include "networking.h"
#include "battleship.c"
#include "msg.h"

struct gameBoard server_board;
struct gameBoard client_board;
struct gameBoard buffer_board;

int main() {
    // Messages settings
    int end_game;
    status_message *s_msg;
    introducion_message *i_msg;
    attack_message *a_msg;

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

    s_msg = malloc(sizeof(status_message));

    recv(client_socket, s_msg, sizeof(status_message), 0);

    if (s_msg->response != 2)
    {
        exit(1);
    }
    free(s_msg);

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
    while(1)
{
    // Server attacks -----------------------------------------------------
    display_my_board(server_board); ///Our map (We have to put our print map)

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
    send(client_socket, a_msg, sizeof(attack_message), 0);
    free(a_msg);

    // Get response (CHECKS if won)
    s_msg = malloc(sizeof(status_message));
    recv(client_socket, s_msg, sizeof(status_message), 0);

    switch (s_msg->response)
    // This is where we find out if the coordinates hit a ship
    {
        case 0: // A miss so nothing happens
            client_map->map[y][x] = MISS; //Change the status
            break;
        case 1: // A hit
            client_map->map[y][x] = HIT; //Change the status
            break;
        case 3: //A win
            system("clear");
            printf("\nYOU WON!!!\n\n");
            close(server_socket);
            close(client_socket);
            exit(1);
        default:
            break;
    }
    free(s_msg);

    system("clear");

    // Client attacks -----------------------------------------------------

    show_map(m); // show my map

    // Receive attack
    printf("Waiting for an attack\n");
    a_msg = malloc(sizeof(attack_message));
    recv(client_socket, a_msg, sizeof(attack_message), 0);

    // Send response
    s_msg = malloc(sizeof(status_message));
    s_msg->type = 3;

    if (attack_ship(m, a_msg->x, a_msg->y) == 1) // if hit
    {
        if (check_map(m) == 0) // Check for win conditions here
        {
            s_msg->response = 3; // This means lost
            send(client_socket, s_msg, sizeof(status_message), 0);
            free(a_msg);
            free(s_msg);
            system("clear");
            printf("\nYOU LOST!!!\n\n");
            close(server_socket);
            close(client_socket);
            exit(1);
        }
        else // It was just a hit
        {
            s_msg->response = 1;
        }
    }
    else { // This means it was a miss
        s_msg->response = 0;
    }
    send(client_socket, s_msg, sizeof(status_message), 0);
    free(a_msg);
    free(s_msg);
    system("clear");
}
}
}
