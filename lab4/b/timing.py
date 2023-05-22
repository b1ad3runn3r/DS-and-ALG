from pwn import *
import matplotlib.pyplot as plt
import numpy as np

pty = process.PTY
io = process('build/lab4b', stdin=pty, stdout=pty)
io.recvuntil(b'Enter option: ')
io.send(b'8\n')

res = io.recvuntil(b'0. Quit')
res = res.split(b'\n')

wanted = []

for i in range(0, len(res) - 1, 2):
    wanted.append(res[i + 1])

# find, insert, delete
r_find = []
r_insert = []
r_delete = []

num_start = 0
cur_test = 0
for i in range(0, 3000, 3):
    num_start += 1000
    cur_test += 1
    cur = wanted[i: i + 3]
    # print(cur)
    if (cur == []):
        break

    for j in range(3):
        tmp = str(cur[j])
        tmp = tmp.replace(f'number of nodes = {num_start}, time = ', '')
        tmp = tmp.replace(f'test #{cur_test}, ', '').replace("'", '')
        tmp = int(tmp.split(', ')[1])
        tmp = tmp / 1000

        if (j == 0):
            r_find.append(tmp)

        elif (j == 1):
            r_insert.append(tmp)

        else:
            r_delete.append(tmp)

r_find = np.array(r_find)
r_insert = np.array(r_insert)
r_delete = np.array(r_delete)

X = list(i * 1000 for i in range(1, 101))
X = np.array(X)

fig, (ax0, ax1, ax2) = plt.subplots(3)
fig.suptitle("Algorithms complexity comparsion")

def fit_logarithm(x, y):
    t = np.log(x)
    p = np.polyfit(t, y, 1)

    a = p[0]
    b = p[1]

    x_fit = np.linspace(np.min(x), np.max(x), 100)
    y_fit = a * np.log(x_fit) + b

    return x_fit, y_fit

new_x, new_y = fit_logarithm(X, r_find)

ax0.plot(X, r_find, 'yo', label='Raw data')
ax0.plot(new_x, new_y, '--k', label='Fitted curve')
ax0.set_title("Find function")
ax0.legend()


new_x, new_y = fit_logarithm(X, r_insert)

ax1.plot(X, r_insert, 'yo', label='Raw data')
ax1.plot(new_x, new_y, '--k', label='Fitted curve')
ax1.set_title("Insert function")
ax1.legend()


new_x, new_y = fit_logarithm(X, r_delete)

ax2.plot(X, r_delete, 'yo', label='Raw data')
ax2.plot(new_x, new_y, '--k', label='Fitted curve')
ax2.set_title("Delete function")
ax2.legend()


plt.show()