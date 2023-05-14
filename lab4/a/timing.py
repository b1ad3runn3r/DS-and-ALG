from pwn import *
import matplotlib.pyplot as plt
import numpy as np

pty = process.PTY
io = process('build/lab4a', stdin=pty, stdout=pty)
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
for i in range(0, 30, 3):
    num_start += 1000000
    cur_test += 1
    cur = wanted[i: i + 3]
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

X = list(i * 1000000 for i in range(1, 11))
X = np.array(X)

fig, (ax0, ax1, ax2) = plt.subplots(3)
fig.suptitle("Algorithms complexity comparsion")

coef = np.polyfit(X, r_find, 1)
polyf = np.poly1d(coef)

ax0.plot(X, r_find, 'yo', X, polyf(X), '--k')
ax0.set_title("Find function")


coef = np.polyfit(X, r_insert, 1)
polyf = np.poly1d(coef)

ax1.plot(X, r_find, 'yo', X, polyf(X), '--k')
ax1.set_title("Insert function")


coef = np.polyfit(X, r_delete, 1)
polyf = np.poly1d(coef)

ax2.plot(X, r_find, 'yo', X, polyf(X), '--k')
ax2.set_title("Delete function")


plt.show()
