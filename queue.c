#include "queue.h"
#include "tile_game.h"

void enqueue(struct queue *q, struct game_state state) 
{
    size_t state_compress = (size_t) serialize(state);
    insert_at_tail(&(q->data), state_compress);
}

struct game_state dequeue(struct queue *q) 
{
    size_t state_compressed = remove_from_head(&(q->data));
    return deserialize(state_compressed);
}

int number_of_moves(struct game_state start)
{
    // to check if state is finished
    uint8_t check[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}, 
        {13, 14, 15, 0}
    };

    //visited list
    
    // initialize
    struct queue game_queue = {0};
    game_queue.data.head = NULL;
    
    // enqueue starting node
    enqueue(&game_queue, start);
    struct game_state node;
    while (game_queue.data.head != NULL)
    {
        // game state we're looking at
        node = dequeue(&game_queue);
        //test, avoid time out
        // if (iteration > 120000)
        // {
        //     free_list(game_queue.data);
        //     return -1;
        // }
        // check if this is final state, if match, return num_step
        int match_count = 0;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                //printf("game state value: %d; check value: %d\n", node.tiles[i][j], check[i][j]);
                if (node.tiles[i][j] == check[i][j])
                {
                    match_count++;
                }
            }
        }
        // printf("\n");
        if (match_count == 16 && check[node.empty_row][node.empty_col] == 0)
        {
            //printf("matched, %d\n", node.num_steps);
            free_list(game_queue.data);
            
            return (int) node.num_steps;
        }

        // enqueue neighbour
        // enqueue move up
        struct game_state next_up = node;
        if (node.empty_row != 3)
        {
            move_up(&next_up);
            enqueue(&game_queue, next_up);
        }
        // move down
        struct game_state next_down = node;
        if (node.empty_row != 0)
        {
            move_down(&next_down);
            enqueue(&game_queue, next_down);
        }
        // move left
        struct game_state next_left = node;
        if (node.empty_col != 3)
        {
            move_left(&next_left);
            enqueue(&game_queue, next_left);
        }
        // move right
        struct game_state next_right = node;
        if (node.empty_col != 0)
        {
            move_right(&next_right);
            enqueue(&game_queue, next_right);
        }

    }

    return -1;

}
