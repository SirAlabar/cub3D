// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   portal_render.c                                    :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: hluiz-ma <hluiz-ma@student.42porto.com>    +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/03/01 16:30:00 by hluiz-ma          #+#    #+#             */
// /*   Updated: 2025/03/01 16:30:00 by hluiz-ma         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include <cub3d.h>

// /*
// ** Clip X range of portal against screen bounds and current view frustum
// ** Returns true if portal is at least partially visible
// */
// bool	clip_portal_range(int *x_min, int *x_max, int view_min, int view_max)
// {
//     ft_printf(YELLOW"Clipping portal range %d-%d against view %d-%d\n"DEFAULT, 
//         *x_min, *x_max, view_min, view_max);
// 	if (*x_max < view_min || *x_min > view_max)
// 		return (false);
// 	if (*x_min < view_min)
// 		*x_min = view_min;
// 	if (*x_max > view_max)
// 		*x_max = view_max;
// 	if (*x_min <= *x_max)
// 		return (true);
//     ft_printf(YELLOW"After clipping: %d-%d\n"DEFAULT, *x_min, *x_max);
// 	return (false);
// }

// /*
// ** Projects a world-space line to screen-space X coordinates
// ** Needed to determine which portion of screen a portal occupies
// */
// bool project_portal_to_screen(t_game *game, t_bsp_line *line, t_view_data *view)
// {
//     t_fixed32 half_width;

//     ft_printf(YELLOW"\nProjecting portal to screen: (%d,%d) -> (%d,%d)\n"DEFAULT,
//         fixed32_to_int(line->start.x), fixed32_to_int(line->start.y),
//         fixed32_to_int(line->end.x), fixed32_to_int(line->end.y));

//     view->t1 = transform_point(line->start, game);
//     view->t2 = transform_point(line->end, game);
    
//     ft_printf(YELLOW"Transformed coords: (%d,%d) -> (%d,%d)\n"DEFAULT,
//         fixed32_to_int(view->t1.x), fixed32_to_int(view->t1.y),
//         fixed32_to_int(view->t2.x), fixed32_to_int(view->t2.y));
    
//     // Check if portal is behind camera
//     if (view->t1.y <= 0 && view->t2.y <= 0)
//     {
//         ft_printf(RED"Portal completely behind camera\n"DEFAULT);
//         return (false);
//     }
    
//     // Handle partial visibility - clamp points behind camera
//     if (view->t1.y <= 0)
//     {
//         ft_printf(YELLOW"Start point behind camera, clamping\n"DEFAULT);
//         view->t1.y = FIXED_POINT_SCALE;
//     }
    
//     if (view->t2.y <= 0)
//     {
//         ft_printf(YELLOW"End point behind camera, clamping\n"DEFAULT);
//         view->t2.y = FIXED_POINT_SCALE;
//     }
    
//     // Convert to screen space
//     half_width = int_to_fixed32(WINDOW_WIDTH >> 1);
    
//     // Project to screen space
//     view->screen_x1 = (WINDOW_WIDTH >> 1) + fixed32_to_int(
//         fixed32_div(fixed32_mul(view->t1.x, half_width), view->t1.y));
//     view->screen_x2 = (WINDOW_WIDTH >> 1) + fixed32_to_int(
//         fixed32_div(fixed32_mul(view->t2.x, half_width), view->t2.y));
    
//     ft_printf(GREEN"Portal projected to screen coords: x1=%d, x2=%d\n"DEFAULT,
//         view->screen_x1, view->screen_x2);
    
//     return (true);
// }

// /*
// ** Find portals in a leaf node of the BSP tree
// ** Identifies which walls are portals and adds them to the queue
// */
// void	find_node_portals(t_bsp_node *node, t_game *game, t_portal_queue *queue, 
//     int current_depth, int view_min, int view_max)
// {
//     int			i;
//     t_bsp_line	*line;
//     t_view_data	view;
//     t_vector_i	portal;
//     int			temp;

//     if (!node || !game || !queue || current_depth >= MAX_PORTAL_DEPTH)
//         return;
//     ft_printf(BLUE"\nSearching for portals in node at depth %d\n"DEFAULT, current_depth);
//     ft_printf(BLUE"View range: %d-%d\n"DEFAULT, view_min, view_max);    
//     i = -1;
//     while (++i < node->num_lines)
//     {
//         line = node->lines[i];
//         if (is_portal(line))
//         {
//             ft_printf(GREEN"Found potential portal to sector %d\n"DEFAULT, 
//                 line->neighbor_sector_id);
//             if (project_portal_to_screen(game, line, &view))
//             {
//                 if (view.screen_x1 > view.screen_x2)
//                 {
//                     temp = view.screen_x1;
//                     view.screen_x1 = view.screen_x2;
//                     view.screen_x2 = temp;
//                 }
//                 portal.x = view.screen_x1;
//                 portal.y = view.screen_x2;
//                 if (clip_portal_range(&portal.x, &portal.y, view_min, view_max))
//                 {
//                     enqueue_portal(queue, line->neighbor_sector_id, 
//                             portal.x, portal.y, current_depth + 1);
//                 }
//             }
//         }
//     }
// }


// /*
// ** Renders portal boundary with different visuals to indicate a portal
// */
// void render_portal_boundary(t_game *game, t_bsp_line *line, t_scanline *buffer, 
//     int x_min, int x_max)
// {
//     t_fixed_vec32 v1, v2;
//     t_fixed32 z1, z2;
//     t_fixed32 px1, px2;
//     int x1, x2, h1, h2;
//     int x, y, top, bottom;
//     int height;
//     t_fixed32 depth;
//     int shade;

//     ft_printf(CYAN"Rendering portal boundary from x=%d to x=%d\n"DEFAULT,
//     x_min, x_max);

//     // Transform to view space
//     v1 = transform_point(line->start, game);
//     v2 = transform_point(line->end, game);

//     // Basic projection similar to walls but with different color/transparency
//     z1 = fix_max(v1.y, FIXED_POINT_SCALE);
//     z2 = fix_max(v2.y, FIXED_POINT_SCALE);

//     // Project to screen X coordinates
//     px1 = fixed32_div(fixed32_mul(v1.x, int_to_fixed32(WINDOW_WIDTH/2)), z1);
//     px2 = fixed32_div(fixed32_mul(v2.x, int_to_fixed32(WINDOW_WIDTH/2)), z2);

//     // Convert to screen coordinates
//     x1 = WINDOW_WIDTH/2 + fixed32_to_int(px1);
//     x2 = WINDOW_WIDTH/2 + fixed32_to_int(px2);

//     // Calculate height based on distance (simplified for portals)
//     h1 = fixed32_to_int(fixed32_div(int_to_fixed32(WINDOW_HEIGHT), z1));
//     h2 = fixed32_to_int(fixed32_div(int_to_fixed32(WINDOW_HEIGHT), z2));

//     // Clipping against X bounds
//     if (x1 < x_min) x1 = x_min;
//     if (x2 > x_max) x2 = x_max;

//     // Render only within the valid range
//     for (x = x1; x <= x2; x++)
//     {
//     // Interpolate based on position
//     t_fixed32 t = fixed32_div(
//     int_to_fixed32(x - x1),
//     int_to_fixed32(x2 - x1 > 0 ? x2 - x1 : 1)
//     );

//     // Interpolate height
//     height = fixed32_to_int(
//     fixed32_add(
//     int_to_fixed32(h1),
//     fixed32_mul(int_to_fixed32(h2 - h1), t)
//     )
//     );

//     // Calculate screen Y coordinates
//     top = (WINDOW_HEIGHT - height) / 2;
//     bottom = top + height;

//     // Clipping against Y bounds
//     if (top < 0) top = 0;
//     if (bottom >= WINDOW_HEIGHT) bottom = WINDOW_HEIGHT - 1;

//     // Clipping against scanline buffer
//     if (top < buffer->y_top[x])
//     top = buffer->y_top[x];
//     if (bottom > buffer->y_bottom[x])
//     bottom = buffer->y_bottom[x];

//     // Only render if visible
//     if (top <= bottom)
//     {
//     // Calculate depth for shading
//     depth = fixed32_add(
//     v1.y,
//     fixed32_mul(
//     fixed32_sub(v2.y, v1.y),
//     t
//     )
//     );

//     // Calculate shade value (brighter for portals)
//     shade = fixed32_to_int(
//     fixed32_div(int_to_fixed32(255 << 6), depth)
//     );
//     shade = shade < 32 ? 32 : (shade > 255 ? 255 : shade);

//     // Draw portal column as blue-tinted to distinguish from walls
//     for (y = top; y <= bottom; y++)
//     {
//     // Portal-specific coloring with blue tint
//     int portal_color = (shade * 3/4) << 16 | (shade * 3/4) << 8 | shade;
//     draw_pixel(game, x, y, portal_color);
//     }

//     // Update scanline buffer
//     buffer->y_top[x] = bottom + 1;
//     }
//     }
// }

// /*
// ** Renders a wall segment with portal flag consideration
// ** Adapts rendering based on whether wall is a portal or solid
// */
// void	render_wall_with_portal_check(t_game *game, t_bsp_line *line, 
//     t_scanline *buffer, t_portal_queue *portal_queue, 
//     int current_depth, int view_min, int view_max)
// {
//     t_view_data	view;
//     t_vector_i	portal;
//     int			temp;

//     ft_printf(YELLOW"\nRendering wall segment with portal check: (%d,%d) -> (%d,%d)\n"DEFAULT,
//         fixed32_to_int(line->start.x), fixed32_to_int(line->start.y),
//         fixed32_to_int(line->end.x), fixed32_to_int(line->end.y));
//     if (is_portal(line))
//     {
//         ft_printf(GREEN"Wall is a portal to sector %d\n"DEFAULT, line->neighbor_sector_id);
//         if (project_portal_to_screen(game, line, &view))
//         {
//             if (view.screen_x1 > view.screen_x2)
//             {
//                 temp = view.screen_x1;
//                 view.screen_x1 = view.screen_x2;
//                 view.screen_x2 = temp;
//             }
//             portal.x = view.screen_x1;
//             portal.y = view.screen_x2;
//             if (clip_portal_range(&portal.x, &portal.y, view_min, view_max))
//             {
//                 ft_printf(GREEN"Adding portal to queue\n"DEFAULT);
//                 enqueue_portal(portal_queue, get_line_neighbor(line), 
//                         portal.x, portal.y, current_depth + 1);
//                 // Draw portal boundary differently if desired
//                 render_portal_boundary(game, line, buffer);
//             }
//         }
//     }
//     else
//     {
//         render_wall_segment(game, line, buffer);
//     }
// }


// /*
// ** Renders sector with portal identification
// ** Main function to render a sector and identify portals for further processing
// */
// void	render_sector_with_portals(t_game *game, int sector_id, 
//     t_portal_queue *portal_queue, int current_depth, 
//     int view_min, int view_max, t_scanline *buffer)
// {
//     t_sector	*sector;
//     int			i;
//     int			linedef_index;
//     t_linedef	*linedef;
//     t_bsp_line	*wall;

//     if (sector_id < 0 || sector_id >= game->map->sector_count || 
//         current_depth >= MAX_PORTAL_DEPTH)
//         return ;

//     sector = &game->map->sectors[sector_id];

//     // First render the floor and ceiling for this sector
//     // render_floor_ceiling(game, sector_id, view_min, view_max, buffer);

//     // Then render all walls, identifying portals as we go
//     for (i = 0; i < sector->linedef_count; i++)
//     {
//         linedef_index = sector->linedefs[i];
//         linedef = &game->map->linedefs[linedef_index];
        
//         // Create a BSP line for this wall
//         wall = create_bsp_line(
//             game->map->vertices[linedef->vertex1].pos,
//             game->map->vertices[linedef->vertex2].pos,
//             linedef->type
//         );
        
//         if (!wall)
//             continue;
        
//         // Set sector information
//         wall->sector_id = sector_id;
        
//         // Determine if front or back side based on which sector we're in
//         if (linedef->front_sidedef >= 0 && 
//             game->map->sidedefs[linedef->front_sidedef].sector == sector_id)
//         {
//             // We're rendering from front side
//             wall->neighbor_sector_id = (linedef->back_sidedef >= 0) ? 
//                 game->map->sidedefs[linedef->back_sidedef].sector : -1;
//         }
//         else if (linedef->back_sidedef >= 0 && 
//                 game->map->sidedefs[linedef->back_sidedef].sector == sector_id)
//         {
//             // We're rendering from back side
//             wall->neighbor_sector_id = (linedef->front_sidedef >= 0) ? 
//                 game->map->sidedefs[linedef->front_sidedef].sector : -1;
            
//             // Swap vertices for correct winding
//             t_fixed_vec32 temp = wall->start;
//             wall->start = wall->end;
//             wall->end = temp;
//         }
        
//         // Render wall segment with portal check
//         render_wall_with_portal_check(game, wall, buffer, portal_queue, 
//             current_depth, view_min, view_max);
        
//         free(wall);
//     }
// }


// /*
// ** Main portal rendering function
// ** Processes the queue of portals and renders each sector as needed
// */
// void render_with_portals(t_game *game, t_scanline *buffer)
// {
//     t_portal_queue *portal_queue;
//     t_portal_item current_portal;
//     int initial_sector;
    
//     ft_printf(CYAN"\n=== Starting Portal-Based Rendering ===\n"DEFAULT);
    
//     // Create portal rendering queue
//     portal_queue = create_portal_queue();
//     if (!portal_queue)
//     {
//         ft_printf(RED"Failed to create portal queue\n"DEFAULT);
//         return;
//     }
    
//     // Start with player's current sector
//     initial_sector = game->p1.sector - game->map->sectors;
    
//     ft_printf(GREEN"Starting in sector %d\n"DEFAULT, initial_sector);
    
//     // Add initial portal for whole screen in starting sector
//     enqueue_portal(portal_queue, initial_sector, 0, WINDOW_WIDTH - 1, 0);
    
//     // Process portal queue
//     while (dequeue_portal(portal_queue, &current_portal))
//     {
//         // Render current sector within current screen range
//         render_sector(game, current_portal.sector_id, portal_queue, 
//             current_portal.depth, current_portal.x_min, current_portal.x_max, buffer);
//     }
    
//     // Clean up
//     free_portal_queue(portal_queue);
// }