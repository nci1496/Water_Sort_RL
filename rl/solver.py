import json
from collections import deque

CAPACITY = 4


def is_solved(state):

    for bottle in state:

        if len(bottle) == 0:
            continue

        if len(bottle) != CAPACITY:
            return False

        if len(set(bottle)) != 1:
            return False

    return True


def can_pour(a, b):

    if not a:
        return False

    if len(b) >= CAPACITY:
        return False

    if not b:
        return True

    return a[-1] == b[-1]


def pour(state, i, j):

    state = [list(b) for b in state]

    color = state[i][-1]

    while state[i] and state[i][-1] == color and len(state[j]) < CAPACITY:
        state[j].append(state[i].pop())

    return tuple(tuple(b) for b in state)


def neighbors(state):

    n = len(state)

    for i in range(n):
        for j in range(n):

            if i == j:
                continue

            if can_pour(state[i], state[j]):
                yield (i, j, pour(state, i, j))


# 读取关卡
with open("../data/level.json", "r") as f:
    level = json.load(f)

start = tuple(tuple(b) for b in level["bottles"])

queue = deque()
queue.append(start)

visited = {start}

parent = {}
move_from = {}

goal = None

while queue:

    cur = queue.popleft()

    if is_solved(cur):
        goal = cur
        break

    for i, j, nxt in neighbors(cur):

        if nxt in visited:
            continue

        visited.add(nxt)

        parent[nxt] = cur
        move_from[nxt] = (i, j)

        queue.append(nxt)

# 回溯解
moves = []

if goal:

    cur = goal

    while cur != start:

        moves.append(move_from[cur])
        cur = parent[cur]

    moves.reverse()

solution = {
    "moves": moves
}

with open("../data/solution.json", "w") as f:
    json.dump(solution, f)

print("Solved in", len(moves), "moves")
