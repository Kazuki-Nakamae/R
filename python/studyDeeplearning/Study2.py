#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Oct  2 19:53:45 2016

@author: kazuki nakamae
"""

import sys, os
sys.path.append(os.pardir) #親ディレクトリのファイルをインポート
import numpy as np
import pickle
from dataset.mnist import load_mnist
from PIL import Image

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

"""
8bit画像表示
@param img 画像データ (numpy.ndarray[][]:整数or浮動点少数)
"""
def img_show(img):
    pil_img = Image.fromarray(np.uint8(img))
    pil_img.show()
    
"""
mnist学習データ取得
@return x_test テスト画像 (numpy.ndarray[][]:整数)
@return t_test テストラベル (numpy.ndarray[]:整数)
"""
def get_data():
    (x_train, t_train), (x_test, t_test) = \
        load_mnist(flatten=True, normalize=True, one_hot_label=False)
    return x_test, t_test

"""
weightの読みこみ (from pickle file)
@return network weightのパラメータ (numpy.ndarray:浮動点少数)
"""
def init_network():
    with open("sample_weight.pkl", 'rb') as f:
        network = pickle.load(f)
    return network

"""
ニュートラルネットワーク処理
@param network weightのパラメータ (numpy.ndarray:浮動点少数)
@param x 入力画像データ (numpy.ndarray[][]:整数or浮動点少数)
@return y 出力値 (numpy.ndarray:浮動点少数)
"""
def predict(network, x):
    W1, W2, W3 = network['W1'],network['W2'],network['W3']
    b1, b2, b3 = network['b1'],network['b2'],network['b3']
    a1 = np.dot(x, W1) + b1
    z1 = sigmoid(a1)
    a2 = np.dot(z1, W2) + b2
    z2 = sigmoid(a2)
    a3 = np.dot(z2, W3) + b3
    y = softmax(a3)
    return y

x, t = get_data()
network = init_network()

batch_size=100 #バッチの数
accuracy_cnt = 0
for i in range(0, len(x), batch_size):
    #予測
    x_batch = x[i:i+batch_size]
    y_batch = predict(network,x_batch)
    p = np.argmax(y_batch, axis=1) #確率が最大のものを取得
    #判定
    accuracy_cnt += np.sum(p == t[i:i+batch_size])
print("Accuracy:" + str(float(accuracy_cnt) / len(x)))