import random

# Define characters
WALL = '#'
EMPTY = ' '
BOX = '$'
GOAL = '.'
PLAYER1 = '@'
PLAYER2 = '&'
BOX_ON_GOAL = '!'

# Create 150x150 grid
size = 250
grid = [[EMPTY for _ in range(size)] for _ in range(size)]

# Add walls around the border
for i in range(size):
    grid[0][i] = WALL  # top
    grid[size-1][i] = WALL  # bottom
    grid[i][0] = WALL  # left
    grid[i][size-1] = WALL  # right

# Add interior walls in a maze-like pattern
for i in range(2, size-2, 4):
    for j in range(2, size-2, 4):
        # Create wall clusters
        if random.random() > 0.3:
            for di in range(3):
                for dj in range(3):
                    if i+di < size-1 and j+dj < size-1:
                        grid[i+di][j+dj] = WALL

# Add random scattered walls
wall_count = 0
while wall_count < 5000:
    i, j = random.randint(1, size-2), random.randint(1, size-2)
    if grid[i][j] == EMPTY:
        grid[i][j] = WALL
        wall_count += 1

# Add goals (many of them)
goal_count = 0
while goal_count < 2000:
    i, j = random.randint(5, size-6), random.randint(5, size-6)
    if grid[i][j] == EMPTY:
        grid[i][j] = GOAL
        goal_count += 1

# Add boxes (many of them, some on goals)
box_count = 0
while box_count < 1800:
    i, j = random.randint(5, size-6), random.randint(5, size-6)
    if grid[i][j] == EMPTY:
        grid[i][j] = BOX
        box_count += 1
    elif grid[i][j] == GOAL and random.random() > 0.7:
        grid[i][j] = BOX_ON_GOAL
        box_count += 1

# Place players in safe areas
# Player 1 - top left area
found_p1 = False
attempts = 0
while not found_p1 and attempts < 1000:
    i, j = random.randint(10, 30), random.randint(10, 30)
    if grid[i][j] == EMPTY:
        grid[i][j] = PLAYER1
        found_p1 = True
    attempts += 1

# Player 2 - bottom right area
found_p2 = False
attempts = 0
while not found_p2 and attempts < 1000:
    i, j = random.randint(size-30, size-10), random.randint(size-30, size-10)
    if grid[i][j] == EMPTY:
        grid[i][j] = PLAYER2
        found_p2 = True
    attempts += 1

# Count elements for report
wall_count = sum(row.count(WALL) for row in grid)
goal_count = sum(row.count(GOAL) for row in grid)
box_count = sum(row.count(BOX) for row in grid)
box_on_goal_count = sum(row.count(BOX_ON_GOAL) for row in grid)
player_count = sum(row.count(PLAYER1) for row in grid) + sum(row.count(PLAYER2) for row in grid)

# Write to file
with open('level_04.txt', 'w', encoding='utf-8') as f:
    for row in grid:
        f.write(''.join(row) + '\n')

print('Level 250x250 created successfully!')
print(f'- Grid size: 250x250 ({250*250:,} total cells)')
print(f'- Walls: {wall_count:,}')
print(f'- Goals: {goal_count:,}')
print(f'- Boxes: {box_count:,}')
print(f'- Boxes on goals: {box_on_goal_count:,}')
print(f'- Players: {player_count}')
print(f'- File size: ~{(250*251):,} characters (including newlines)')