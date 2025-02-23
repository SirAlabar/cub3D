#!/usr/bin/env python3
import omg
import struct

def map_textures(floor_texture, ceiling_texture, light_value):
    # Doom texture mapping to custom textures
    texture_map = {
        'FLOOR4_8': 'assets/texture/brick1.xpm',
        'CEIL3_5': 'assets/texture/bricks2.xpm',
        'FLAT18': 'assets/texture/bricks3.xpm',
        'TLITE6_4': 'assets/texture/stone1.xpm',
        'FLAT20': 'assets/texture/stone2.xpm',
        'FLOOR7_1': 'assets/texture/stone3.xpm',
        'CEIL5_1': 'assets/texture/stone4.xpm',
        'FLAT14': 'assets/texture/bricks2.xpm',
        'NUKAGE3': 'assets/texture/metal_floor.xpm',
        'TLITE6_5': 'assets/texture/metal_floor.xpm',
        'FLAT5_5': 'assets/texture/metal_floor2.xpm',
        'FLOOR5_1': 'assets/texture/brick1.xpm',
        'FLOOR5_2': 'assets/texture/brick1.xpm',
        'CEIL5_2': 'assets/texture/bricks3.xpm',
        'STEP2': 'assets/texture/bricks3.xpm',
        'FLOOR1_1': 'assets/texture/bricks2.xpm',
        'F_SKY1': 'assets/texture/skybox1.xpm',
        'FLOOR7_2': 'assets/texture/brick1.xpm',
        'TLITE6_6': 'assets/texture/metal_floor2.xpm',
    }
    
    # Check if textures are in the mapping
    floor_tex = texture_map.get(floor_texture, floor_texture)
    ceiling_tex = texture_map.get(ceiling_texture, ceiling_texture)
    
    return floor_tex, ceiling_tex, light_value

def convert_linedef(line):
    # Remove spaces and split the line
    parts = line.strip().split('=')
    
    # Extract linedef number
    linedef_num = parts[0].strip()
    
    # Split values
    values = parts[1].strip().split(',')
    
    # Determine vertices
    start_vertex = 'v' + values[0]
    end_vertex = 'v' + values[1]
    
    # Determine sidedefs and flags
    if len(values) > 2:
        # If the third value is a number, assume it's the back_sidedef
        if values[2].startswith('-'):
            front_sidedef = 'sd' + values[1]
            back_sidedef = values[2] if values[2] != '-1' else '-1'
            wall_type = values[3] if len(values) > 3 else '0'
        else:
            # Otherwise, assume it's the sidedef
            front_sidedef = 'sd' + values[2]
            back_sidedef = 'sd' + values[3] if len(values) > 3 and values[3] != '-1' else '-1'
            wall_type = values[4] if len(values) > 4 else '0'
    else:
        front_sidedef = 'sd' + values[1]
        back_sidedef = '-1'
        wall_type = '0'
    
    # Format: lN = v0,v1,front_sidedef,back_sidedef,type
    return f"{linedef_num} = {start_vertex},{end_vertex},{front_sidedef},{back_sidedef},{wall_type}"

def extract_wad_info(wad_path, output_path, map_name=None):
    # Open the WAD file
    wad = omg.WAD(wad_path)
    
    # If no map name is provided, use the first map
    if map_name is None:
        map_name = list(wad.maps.keys())[0]
    
    # Check if the map exists in the WAD
    if map_name not in wad.maps:
        print(f"Error: Map '{map_name}' not found in WAD.")
        print("Available maps:")
        for name in wad.maps.keys():
            print(name)
        return
    
    map_data = wad.maps[map_name]
    
    with open(output_path, 'w') as f:
        # Write VERTICES
        f.write("[VERTICES]\n")
        vertex_data = map_data['VERTEXES'].data
        vertices = []
        for i in range(0, len(vertex_data), 4):
            x, y = struct.unpack('<hh', vertex_data[i:i+4])
            vertices.append((x, y))
            f.write(f"v{i//4} = {x},{y}\n")
        
        # Write SIDEDEFS
        f.write("\n[SIDEDEFS]\n")
        sidedef_data = map_data['SIDEDEFS'].data
        for i in range(0, len(sidedef_data), 30):
            x_off, y_off = struct.unpack('<hh', sidedef_data[i:i+4])
            upper = sidedef_data[i+4:i+12].decode('ascii').strip('\0')
            lower = sidedef_data[i+12:i+20].decode('ascii').strip('\0')
            middle = sidedef_data[i+20:i+28].decode('ascii').strip('\0')
            sector = struct.unpack('<h', sidedef_data[i+28:i+30])[0]
            f.write(f"sd{i//30} = {x_off},{y_off},{upper},{middle},{lower},s{sector}\n")
        
        # Write LINEDEFS with custom conversion
        f.write("\n[LINEDEFS]\n")
        linedef_data = map_data['LINEDEFS'].data
        for i in range(0, len(linedef_data), 14):
            start, end, flags, special, tag, right_side, left_side = struct.unpack('<hhhhhhh', linedef_data[i:i+14])
            # Add custom line conversion logic
            line_str = f"l{i//14} = {start},{end},{right_side},{left_side},{special}"
            converted_line = convert_linedef(line_str)
            f.write(converted_line + '\n')
        
        # Write SECTORS with texture mapping
        f.write("\n[SECTORS]\n")
        sector_data = map_data['SECTORS'].data
        for i in range(0, len(sector_data), 26):
            floor_h, ceil_h = struct.unpack('<hh', sector_data[i:i+4])
            floor_tex = sector_data[i+4:i+12].decode('ascii').strip('\0')
            ceil_tex = sector_data[i+12:i+20].decode('ascii').strip('\0')
            light, special, tag = struct.unpack('<hhh', sector_data[i+20:i+26])
            
            # Apply texture mapping
            floor_tex, ceil_tex, light = map_textures(floor_tex, ceil_tex, light)
            
            f.write(f"s{i//26} = {floor_h},{ceil_h},{light},{floor_tex},{ceil_tex}\n")
        
        # Write THINGS
        f.write("\n[THINGS]\n")
        thing_data = map_data['THINGS'].data
        for i in range(0, len(thing_data), 10):
            x, y, angle, type_id, flags = struct.unpack('<hhhhh', thing_data[i:i+10])
            f.write(f"{type_id},{x},{y},{angle}\n")
    
    print(f"Information extracted and saved to {output_path}")

def main():
    # Path to WAD file and output file
    wad_file_path = './DOOM1.WAD'
    output_file_path = 'output_map.txt'
    
    # Optional: Specify a specific map name
    # map_name = 'E1M1'  # Uncomment and set to the desired map name
    map_name = 'E1M5'  # Uses the first map by default

    # Call the function
    extract_wad_info(wad_file_path, output_file_path, map_name)

if __name__ == '__main__':
    main()