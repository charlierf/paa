import random

def generate_maze(rows, cols):
    maze = [[0] * cols for _ in range(rows)]  # Inicializa todas as células como espaços vazios

    # Define as bordas como paredes
    for row in range(rows):
        maze[row][0] = 1
        maze[row][cols - 1] = 1
    for col in range(cols):
        maze[0][col] = 1
        maze[rows - 1][col] = 1

    if random.random() < 0.5:
        random_row = random.randint(1, rows - 2)
        random_col = random.randint(1, cols - 2)
        options = [(random_row, 0), (random_row, cols-1), (0, random_col), (rows-1, random_col)]
        random_row, random_col = random.choice(options)
        maze[random_row][random_col] = 0
    
    # Define a posição inicial aleatória
    start_row = random.randint(1, rows - 2)
    start_col = random.randint(1, cols - 2)
    maze[start_row][start_col] = 'X'  # Define o ponto de partida
    
    # Define aleatoriamente uma saída nas bordas
    border = random.choice(['top', 'bottom', 'left', 'right'])
    if border == 'top':
        exit_row = 0
        exit_col = random.randint(0, cols - 1)
    elif border == 'bottom':
        exit_row = rows - 1
        exit_col = random.randint(0, cols - 1)
    elif border == 'left':
        exit_row = random.randint(0, rows - 1)
        exit_col = 0
    else:  # right
        exit_row = random.randint(0, rows - 1)
        exit_col = cols - 1
        
    maze[exit_row][exit_col] = 0  # Define a saída
    
    # Gera caminhos aleatórios
    for row in range(rows):
        for col in range(cols):
            if maze[row][col] == 'X': #or maze[row][col] == 0:  # Se for a posição inicial ou a saída, continua
                continue
                
            # Define aleatoriamente se a célula será uma parede ou um espaço vazio
            if random.random() < 0.6:  # 30% de chance de ser uma parede
                maze[row][col] = 1
            else:
                if row != 0 and row != rows - 1 and col != 0 and col != cols - 1:
                    maze[row][col] = 0
                
    return maze

def print_maze(maze):
    print(cols,rows)
    for row in maze:
        print(' '.join(map(str, row)))

# Teste da função
rows = random.randint(5, 20)
cols = random.randint(5, 20)
qtd = 6
print(qtd)
for i in range(qtd):
    maze = generate_maze(rows, cols)
    print_maze(maze)
  
# maze = generate_maze(rows, cols)
# print_maze(maze)
