# -*- coding: utf-8 -*-
"""
Created on Sun Sep 22 09:00:45 2019

@author: kelil
"""


import matplotlib.pyplot as plt
import matplotlib.image as mpimg
#matplotlib.use('Agg')
import os
from keras.models import load_model
import keras
import numpy as np
#from PIL import Image
#import mobilenet_v2_keras
from keras.utils.generic_utils import CustomObjectScope
from keras.applications.imagenet_utils import decode_predictions
import cv2
from picamera import PiCamera
import time
import serial 

camera= PiCamera()
camera.resolution =(1080,1080)
camera.framerate = 60
cv2.namedWindow('test', cv2.WINDOW_NORMAL)
cv2.resizeWindow('test',320, 320)
for i in range(1,5):
    camera.capture('/home/pi/prediction/paper/test.jpg')
    capimg= cv2.imread('/home/pi/prediction/paper/test.jpg')
    cv2.imshow("test",capimg)
    cv2.waitKey(300)
#加载模型h5文件
#with CustomObjectScope({'relu6': mobilenet_v2_keras.relu6,'DepthwiseConv2D': mobilenet_v2_keras.DepthwiseConv2D}):
   # model = load_model('weights.h5')
model = load_model("kares-03.h5")
#model.load_weights("‪F:\\深度学习\\kares-03.h5", by_name=False)
model.summary()
ser = serial.Serial('/dev/ttyACM0', 9600,timeout=1)
#response = ser.readall()
#规范化图片大小和像素值
def get_inputs(src=[]):
    pre_x = []
    for s in src:
        input = cv2.imread(s)
        #capimg = cv2.cvtColor(input,cv2.COLOR_BGR2RGB)
        input = cv2.resize(input, (224, 224))
        input = cv2.cvtColor(input, cv2.COLOR_BGR2RGB)
        pre_x.append(input)  # input一张图片
    pre_x = np.array(pre_x) / 255.0
    return pre_x
#要预测的图片保存在这里
predict_dir = "/home/pi/prediction"

test = os.listdir(predict_dir)

print(test)
#新建一个列表保存预测图片的地址

images = []
#获取每张图片的地址，并保存在列表images中
predict = True
labels={"0":"battery", "1":"chopsticks", "2":"fruit","3":"paper","4":"pop_can","5":"powder_box","6":"sponge","7":"water_bottle"}#分类标签
box_labels={"0":"recycled","1":"hazardous_waste","2":"kitchen_garbage","3":"other"}#垃圾桶分类标签
recycled = ['pop_can','water_bottle','paper']
hazardous_waste = ['powder_box','battery']
kitchen_garbage = ['fruit']
other = ['chopsticks','sponge']

while (predict == True):
    #size = ser.inWaiting()
    #if (size!=0):
    response = ser.readall()#用response读取端口的返回值
    response = response.decode()#串口传过来是bytes要转成str
    cap = "cap"
    print(response)
    while (cap in response):#当收到拍照命令时拍照并预测
        
        
        camera.capture('/home/pi/prediction/paper/test.jpg')
        capimg= cv2.imread('/home/pi/prediction/paper/test.jpg')
        for testpath in test:
            for fn in os.listdir(os.path.join(predict_dir, testpath)):
                if fn.endswith('jpg'):
                    fd = os.path.join(predict_dir, testpath, fn)
                    print(fd)
                    images.append(fd)
        #调用函数，规范化图片
        pre_x = get_inputs(images)
        #预测
        
        
        #key = cv2.waitkey(delay=1)
        pre_y = np.argmax(model.predict(pre_x), axis=1)#将行的最大值输出来并写上类别
        testlabel = labels[str(pre_y[0])]#
        
        print(testlabel)
        #capimg= cv2.imread('/home/pi/prediction/paper/test.jpg')
        cv2.putText(capimg,testlabel,(250,80),cv2.FONT_HERSHEY_SIMPLEX,3,(0,0,255),2)#显示文字
        
        cv2.imshow("test",capimg)
        cv2.waitKey(300)
        ser.flushInput()
        if testlabel in recycled:
            ser.write('0'.encode("utf-8"))
        elif testlabel in hazardous_waste:
            ser.write('1'.encode("utf-8"))
        elif testlabel in kitchen_garbage:
            ser.write('2'.encode("utf-8"))
        elif testlabel in other:
            ser.write('3'.encode("utf-8"))
        print(pre_y)
        time.sleep(1)
        #用response读取端口的返回值
        #cv2.destroyAllWindows()
        print(response)
        
        images=[]#清除图像列表
        response=[]
    
        
    #print('Predicted:', decode_predictions(pre_y, top=3)[0])

    #pre_y = pre_y[0][0]
    #print('%s(%.2f%%)'%(pre_y[1],pre_y[2]))



