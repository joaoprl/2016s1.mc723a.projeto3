#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt

def chart(inds, labs, vals, filepath):
    fig = plt.figure(facecolor='white')
    ax = fig.add_subplot(1, 1, 1)
    ax.bar(inds, vals, .5)
    ax.set_xticks(inds + .25)
    ax.set_xticklabels(labs, rotation=15)
    ax.set_ylabel('total de ciclos')
    ax.yaxis.grid()

    fig.savefig(filepath)
    # plt.show()


def main():
    idx_name = 0

    inds = np.arange(5)
    labs = ('serial', 'sem per., sem par.', 'sem per., com par.', 'com per., sem par.', 'com per., com par.')

    vals = np.array([1451588590, 2723180964, 2723145675, 1846109882, 1845981614], dtype=int)

    chart(inds, labs, vals, '%d.png' % idx_name)
    idx_name += 1

    inds = np.arange(4)
    labs = ('sem per., sem par.', 'sem per., com par.', 'com per., sem par.', 'com per., com par.')

    vals = np.array([1271592374, 317889271, 394521292, 98598256], dtype=int)

    chart(inds, labs, vals, '%d.png' % idx_name)
    idx_name += 1

    vals = np.array([2723180964, 1769477861, 1846109882, 1550186846], dtype=int)

    chart(inds, labs, vals, '%d.png' % idx_name)


if __name__ == '__main__':
    main()
