#!/usr/bin/env python3

def convert_linedef(line):
    # Remove espaços e divide a linha
    parts = line.strip().split('=')
    
    # Extrai o número do linedef
    linedef_num = parts[0].strip()
    
    # Divide os valores
    values = parts[1].strip().split(',')
    
    # Determina os vértices
    start_vertex = 'v' + values[0]
    end_vertex = 'v' + values[1]
    
    # Determina sidedefs e flags
    if len(values) > 2:
        # Se o terceiro valor for um número, assume que é o back_sidedef
        if values[2].startswith('-'):
            front_sidedef = 'sd' + values[1]
            back_sidedef = values[2] if values[2] != '-1' else '-1'
            wall_type = values[3] if len(values) > 3 else '0'
        else:
            # Senão, assume que é o sidedef
            front_sidedef = 'sd' + values[2]
            back_sidedef = 'sd' + values[3] if len(values) > 3 and values[3] != '-1' else '-1'
            wall_type = values[4] if len(values) > 4 else '0'
    else:
        front_sidedef = 'sd' + values[1]
        back_sidedef = '-1'
        wall_type = '0'
    
    # Formato: lN = v0,v1,front_sidedef,back_sidedef,type
    return f"{linedef_num} = {start_vertex},{end_vertex},{front_sidedef},{back_sidedef},{wall_type}"

def main():
    # Arquivo de entrada e saída
    input_file = 'original_linedefs.txt'
    output_file = 'converted_linedefs.txt'
    
    # Lê o arquivo de entrada
    with open(input_file, 'r') as f:
        lines = f.readlines()
    
    # Filtra e converte linedefs
    linedefs = [line.strip() for line in lines if line.startswith('l')]
    
    # Converte linedefs
    converted_linedefs = ['[LINEDEFS]']
    converted_linedefs.extend(convert_linedef(line) for line in linedefs)
    
    # Salva o resultado
    with open(output_file, 'w') as f:
        f.write('\n'.join(converted_linedefs))
    
    print(f"Conversão concluída. Verifique {output_file}")

if __name__ == '__main__':
    main()