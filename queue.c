#include "queue.h"
#include "tile_game.h"

void enqueue(struct queue *q, struct game_state state) 
{
    struct linked_list data = q->data;
    size_t state_compress = (size_t) serialize(state);
    insert_at_tail(&data, state_compress);
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

        // check if this is final state, if match, return num_step
        int match_count = 0;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (node.tiles[i][j] == check[i][j])
                {
                    match_count++;
                }
            }
        }
        if (match_count == 16 && check[node.empty_row][node.empty_col] == 0)
        {
            return node.num_steps;
        }

        // enqueue neighbour
        // enqueue move up
        if (node.empty_row != 3)
        {
            move_up(&node);
            enqueue(&game_queue, node);
            move_down(&node);
        }
        // move down
        if (node.empty_row != 0)
        {
            move_down(&node);
            enqueue(&game_queue, node);
            move_up(&node);
        }
        // move left
        if (node.empty_col != 3)
        {
            move_left(&node);
            enqueue(&game_queue, node);
            move_right(&node);
        }
        // move right
        if (node.empty_col != 0)
        {
            move_right(&node);
            enqueue(&game_queue, node);
            move_left(&node);
        }

    }

    return -1;

}
