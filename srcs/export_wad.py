import omg
import struct

def extract_wad_info(wad_path, output_path):
    wad = omg.WAD(wad_path)
    map_name = list(wad.maps.keys())[0] 
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
        
        # Write LINEDEFS
        f.write("\n[LINEDEFS]\n")
        linedef_data = map_data['LINEDEFS'].data
        for i in range(0, len(linedef_data), 14):
            start, end, flags, special, tag, right_side, left_side = struct.unpack('<hhhhhhh', linedef_data[i:i+14])
            f.write(f"l{i//14} = {start},{end},{right_side},{left_side},{special}\n")
        
        # Write SECTORS
        f.write("\n[SECTORS]\n")
        sector_data = map_data['SECTORS'].data
        for i in range(0, len(sector_data), 26):
            floor_h, ceil_h = struct.unpack('<hh', sector_data[i:i+4])
            floor_tex = sector_data[i+4:i+12].decode('ascii').strip('\0')
            ceil_tex = sector_data[i+12:i+20].decode('ascii').strip('\0')
            light, special, tag = struct.unpack('<hhh', sector_data[i+20:i+26])
            f.write(f"s{i//26} = {floor_h},{ceil_h},{light},{floor_tex},{ceil_tex}\n")
        
        # Write THINGS
        f.write("\n[THINGS]\n")
        thing_data = map_data['THINGS'].data
        for i in range(0, len(thing_data), 10):
            x, y, angle, type_id, flags = struct.unpack('<hhhhh', thing_data[i:i+10])
            f.write(f"{type_id},{x},{y},{angle}\n")
    
    print(f"Information extracted and saved to {output_path}")

# Path to WAD file and output file
wad_file_path = './DOOM1.WAD'
output_file_path = 'output_map.txt'

# Call the function
extract_wad_info(wad_file_path, output_file_path)