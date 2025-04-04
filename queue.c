#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>

void enqueue(struct queue *q, struct game_state state) 
{
    insert_at_tail(&(q->data), (size_t) serialize(state));
}

struct game_state dequeue(struct queue *q) 
{
    return deserialize(remove_from_head(&(q->data)));
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
    size_t visited_size = 2048;
    size_t visited_count = 0;
    size_t* visited = malloc(sizeof(size_t) * visited_size);
    // initialize
    struct queue game_queue = {0};
    game_queue.data.head = NULL;
    
    // enqueue starting node
    enqueue(&game_queue, start);
    visited[visited_count++] = serialize(start);
    struct game_state node;
    while (game_queue.data.head != NULL)
    {
        // game state we're looking at
        node = dequeue(&game_queue);
        //test, avoid time out
        if (node.num_steps > 100)
        {
            free_list(game_queue.data);
            free(visited);
            return -1;
        }
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
            free(visited);
            return (int) node.num_steps;
        }

        // enqueue neighbour
        // enqueue move up
        
        if (node.empty_row != 3)
        {
            struct game_state next_up = node;
            char seen = 0;
            move_up(&next_up);
            size_t next_up_code = serialize(next_up);
            for (size_t i = 0; i < visited_count; i++)
            {
                if (next_up_code == visited[i])
                {
                    seen = 1;
                    break;
                }
            }
            if (!seen)
            {
                visited[visited_count++] = next_up_code;
                enqueue(&game_queue, next_up);
                if (visited_count == visited_size)
                {
                    visited_size *= 2;
                    visited = realloc(visited, visited_size * sizeof(size_t));
                }
            }
        }

        if (node.empty_row != 0)
        {
            struct game_state next_down = node;
            char seen = 0;
            move_down(&next_down);
            size_t next_down_code = serialize(next_down);
            for (size_t i = 0; i < visited_count; i++)
            {
                if (next_down_code == visited[i])
                {
                    seen = 1;
                    break;
                }
            }
            if (!seen)
            {
                visited[visited_count++] = next_down_code;
                enqueue(&game_queue, next_down);
                if (visited_count == visited_size)
                {
                    visited_size *= 2;
                    visited = realloc(visited, visited_size * sizeof(size_t));
                }
            }
        }

        if (node.empty_col != 3)
        {
            struct game_state next_left = node;
            char seen = 0;
            move_left(&next_left);
            size_t next_left_code = serialize(next_left);
            for (size_t i = 0; i < visited_count; i++)
            {
                if (next_left_code == visited[i])
                {
                    seen = 1;
                    break;
                }
            }
            if (!seen)
            {
                visited[visited_count++] = next_left_code;
                enqueue(&game_queue, next_left);
                if (visited_count == visited_size)
                {
                    visited_size *= 2;
                    visited = realloc(visited, visited_size * sizeof(size_t));
                }
            }
        }

        if (node.empty_col != 0)
        {
            struct game_state next_right = node;
            char seen = 0;
            move_right(&next_right);
            size_t next_right_code = serialize(next_right);
            for (size_t i = 0; i < visited_count; i++)
            {
                if (next_right_code == visited[i])
                {
                    seen = 1;
                    break;
                }
            }
            if (!seen)
            {
                visited[visited_count++] = next_right_code;
                enqueue(&game_queue, next_right);
                if (visited_count == visited_size)
                {
                    visited_size *= 2;
                    visited = realloc(visited, visited_size * sizeof(size_t));
                }
            }
        }

    }
    //printf("cannot solve");
    free_list(game_queue.data);
    free(visited);
    return -1;

}
