#!/usr/bin/env python3
def map_textures(floor_texture, ceiling_texture, light_value):
    # Mapeamento de texturas do Doom para suas texturas
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
    
    # Verifica se as texturas estão no mapeamento
    floor_tex = texture_map.get(floor_texture, floor_texture)
    ceiling_tex = texture_map.get(ceiling_texture, ceiling_texture)
    
    return floor_tex, ceiling_tex, light_value

def convert_sectors(input_file, output_file):
    with open(input_file, 'r') as f:
        lines = f.readlines()
   
    converted_sectors = ['[SECTORS]']
   
    for line in lines:
        if line.startswith('s'):
            # Divide a linha do setor
            parts = line.strip().split('=')
            sector_num = parts[0].strip()
            values = parts[1].strip().split(',')
           
            # Mapeia as texturas e luz
            floor_height = values[0]
            ceiling_height = values[1]
            light_value = values[2]
            floor_texture = values[3]
            ceiling_texture = values[4]
            
            # Converte as texturas
            floor_tex, ceiling_tex, light = map_textures(floor_texture, ceiling_texture, light_value)
           
            # Monta a nova linha do setor
            new_line = f"{sector_num} = {floor_height},{ceiling_height},{light},{floor_tex},{ceiling_tex}"
            converted_sectors.append(new_line)
   
    # Salva o resultado
    with open(output_file, 'w') as f:
        f.write('\n'.join(converted_sectors))
   
    print(f"Conversão concluída. Verifique {output_file}")

def main():
    input_file = 'paste.txt'
    output_file = 'converted_sectors.txt'
    convert_sectors(input_file, output_file)

if __name__ == '__main__':
    main()