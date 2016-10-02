# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pylab as plt


"""
ステップ関数
@param x 入力値 (numpy.ndarray:整数or浮動点少数)
@return y 出力値 (numpy.ndarray:整数 0or1)
"""
def step_function(x):
    return np.array(x > 0, dtype=np.int)
    
"""
シグモイド関数
@param x 入力値 (numpy.ndarray:整数or浮動点少数)
@return y 出力値 (numpy.ndarray:浮動点少数)
"""
def sigmoid(x):
    return 1 / ( 1 + np.exp(-x) )
    
"""
ソフトマックス関数
@param x 入力値 (numpy.ndarray:整数or浮動点少数)
@return y 出力値 (numpy.ndarray:浮動点少数)
"""
def softmax(x):
    c = np.max(x)
    exp_x = np.exp(x - c)#オーバーフロー対策
    sum_exp_x = np.sum(exp_x)
    y = exp_x / sum_exp_x
    return y

x = np.arange(-5.0,5.0,0.1)
y1 = sigmoid(x)
y2 = step_function(x)
y3 = softmax(x)
plt.plot(x, y1, label="sigmoid")
plt.plot(x, y2, label="step", linestyle="--")
plt.plot(x, y3, label="softmax", linestyle=":")
plt.ylim(-0.1, 1.1)#表示する値域
plt.legend(loc=2)
plt.show()