# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'C:\Users\darney\New Folder\update\firstui.ui'
#
# Created by: PyQt4 UI code generator 4.11.4
#
# WARNING! All changes made in this file will be lost!
#import random
from PyQt4 import QtCore, QtGui, QtNetwork
import armPkg
try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)
        
class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        self.fortunes = (
        "You've been leading a dog's life. Stay off the furniture.",
        "You've got to think about tomorrow.",
        "You will be surprised by a loud noise.",
        "You will feel hungry again in another hour.",
        "You might have mail.",
        "You cannot kill time without injuring eternity.",
        "Computers are not intelligent. They only think they are.")
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.setFixedSize(400, 300)
        print(type(MainWindow))
        
        self.centralWidget = QtGui.QWidget(MainWindow)
        self.centralWidget.setObjectName(_fromUtf8("centralWidget"))
        self.centralWidget.setWindowIcon(QtGui.QIcon(QtGui.QPixmap("icons/cubed.png")));
        self.textBrowser = QtGui.QTextBrowser(self.centralWidget)
        self.textBrowser.setGeometry(QtCore.QRect(90, 10, 300,100))
        self.textBrowser.setObjectName(_fromUtf8("textBrowser"))
        self.pushButton = QtGui.QPushButton(self.centralWidget)
        self.pushButton.setGeometry(QtCore.QRect(10, 260, 75, 23))
        self.pushButton.setObjectName(_fromUtf8("pushButton"))

        self.sendButton = QtGui.QPushButton(self.centralWidget)
        self.sendButton.setGeometry(QtCore.QRect(10, 10, 75, 23))
        self.sendButton.setObjectName(_fromUtf8("sendButton"))
        
        self.openFileNameButton = QtGui.QPushButton(self.centralWidget)
        self.openFileNameButton.setGeometry(QtCore.QRect(10, 40, 75, 23))
        self.openFileNameButton.setObjectName(_fromUtf8("openFilebutton"))
        #表格
        self.socketchat = QtGui.QTableWidget(4, 3, self.centralWidget)
        self.socketchat.setGeometry(QtCore.QRect(90, 130, 300, 150))
        self.socketchat.setHorizontalHeaderItem(0, QtGui.QTableWidgetItem("USER ID"))
        #self.socketchat.horizontalHeaderItem(0).setBackgroundColor(QtGui.QColor(255,255,255))
        self.socketchat.setHorizontalHeaderItem(1, QtGui.QTableWidgetItem("IP ADDRESS"))
        self.socketchat.setHorizontalHeaderItem(2, QtGui.QTableWidgetItem("PORT"))
        #self.socketchat.autoFillBackground()
        self.socketchat.setSelectionBehavior(QtGui.QAbstractItemView.SelectRows)
        self.socketchat.setEditTriggers(QtGui.QAbstractItemView.NoEditTriggers);
        self.socketchat.verticalHeader().setVisible(0)
#        self.socketchat.setItem(0,0,QtGui.QTableWidgetItem("Jan"));  
#        self.socketchat.setItem(0,1,QtGui.QTableWidgetItem("Feb"));  
#        self.socketchat.setItem(0,2,QtGui.QTableWidgetItem("Mar")); 
        
        
        
        self.native = QtGui.QCheckBox()
        self.native.setText("Use native file dialog.")
        self.native.setChecked(True)
        MainWindow.setCentralWidget(self.centralWidget)
        
        
        self.socketlist=[]
        self.validlist=[]
        self.tcp=QtNetwork.QTcpServer()
        self.tcp.listen(QtNetwork.QHostAddress.Any, 9001);
        #self.tcp.waitForNewConnection(-1)

        self.retranslateUi(MainWindow)
        #self.tcp.newConnection.connect(self.newConnect)
        #打开文件获取文件名 signal
        self.openFileNameButton.clicked.connect(self.setOpenFileName)
        QtCore.QObject.connect(self.tcp, QtCore.SIGNAL(_fromUtf8("newConnection()")), self.newConnect)
#        QtCore.QObject.connect(self.textBrowser, QtCore.SIGNAL("didSomething"), self.update_label) 
        QtCore.QObject.connect(self.pushButton, QtCore.SIGNAL(_fromUtf8("clicked()")), self.textLableshowText)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)
        self.do_something()
        self.Timer_5S()
    def DebugWarning(self,MESSAGE="Warning"):
        msgBox=QtGui.QMessageBox(QtGui.QMessageBox.Warning,"Warning", MESSAGE,QtGui.QMessageBox.NoButton, self.centralWidget)
        msgBox.addButton("OK", QtGui.QMessageBox.AcceptRole)
        if msgBox.exec_() == QtGui.QMessageBox.AcceptRole:
            pass
    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow", None))
        self.pushButton.setText(_translate("MainWindow", "清空", None))
        self.sendButton.setText(_translate("MainWindow", "发送", None))
        self.openFileNameButton.setText(_translate("MainWindow", "打开文件", None))
        file = QtCore.QFile('new.qss')
        file.open(QtCore.QFile.ReadOnly)
        styleSheet = file.readAll()
        print(type(styleSheet))
        styleSheet = str(styleSheet, encoding='utf8')
        QtGui.qApp.setStyleSheet(styleSheet)
        
        
        
    def ConnectUpdate(self):
        listlen=len(self.socketlist)
        #清空表格内容不包括表头
        self.socketchat.clearContents()
        if listlen == 0:
            return
        for c in range(listlen):
            self.socketchat.setItem(c,0,QtGui.QTableWidgetItem(str(c+1)));  
            self.socketchat.setItem(c,1,QtGui.QTableWidgetItem(self.socketlist[c].peerAddress().toString()));  
            self.socketchat.setItem(c,2,QtGui.QTableWidgetItem(str(self.socketlist[c].peerPort())));
            
    def Timer_5S(self):
        self.time_5s=QtCore.QTimer()
        self.time_5s.timeout.connect(self.socketCheck)
        self.time_5s.start(5000);
        print('Timer START')
    def socketCheck(self):
        print('Timer accept')
        dellist=[]
        listlen=len(self.socketlist)
        print(listlen)
        for c in self.socketlist:
            if not c.isValid:
                print(c)
                dellist.append(c)
                c.close()
                try:
                    self.socketlist.remove(c)
                except:
                    pass


                
        if len(dellist)==0:
            return
        print('remove list socket')
        if listlen == 0:
            self.socketchat.clear()
            return
        self.ConnectUpdate()
    def newConnect(self):
        self.tcpsocket=self.tcp.nextPendingConnection()
        self.socketlist.append(self.tcpsocket)
        print(len(self.socketlist))
        self.tcpsocket.readyRead.connect(self.readMessage)
        #self.tcpsocket.disconnected.connect(self.tcpsocket.deleteLater)
        self.tcpsocket.disconnected.connect(self.socketclose)
        self.ConnectUpdate()
        print(self.tcpsocket.localAddress().toString())
        print(self.tcpsocket.localPort())
    def socketclose(self):
        #self.socketlist.index(3)
        try:
            self.tcpsocket.close()
            self.socketlist.remove(self.tcpsocket)
        except:
            pass
        print(len(self.socketlist))
        print('close')

        self.ConnectUpdate()
        
    def readMessage(self):
        qba=self.tcpsocket.readAll()
        qs1 = str(qba, encoding = "ascii", errors='ignore')
        self.textBrowser.append(_translate(qs1, qs1, None))
        #self.sendFortune(self.tcpsocket, '')
        
        
    def sendFortune(self, socket, data):
        if not socket.isValid():
            self.DebugWarning("connect socket is not valid warning")
        try:
            # Python v3.
            fortune = bytes(data, encoding='ascii')
        except:
            # Python v2.
            pass
        #print(fortune)

        try:
            socket.write(fortune)
        except:
            print("error")
            

    def setOpenFileName(self):    
        options = QtGui.QFileDialog.Options()
        if not self.native.isChecked():
            options |= QtGui.QFileDialog.DontUseNativeDialog
        self.fileName = QtGui.QFileDialog.getOpenFileName(self.centralWidget,
                "QFileDialog.getOpenFileName()",
                '', 
                "All Files (*);;Text Files (*.txt)", options)
        if self.fileName:
            #print(self.fileName)
            self.textBrowser.append(self.fileName)
            package1=armPkg.arm_package_update_assign(self.fileName)
            #发送校验包
            try:
                self.sendFortune(self.tcpsocket,package1)
            except:
                self.DebugWarning("tcp connect error!")
#发送包一 以及 传输MD5校验值
#            self
#            sendFortune(self, socket, data)
        

        


    #产生信号量
    def do_something(self): 
        QtCore.QObject.emit(self.textBrowser,QtCore.SIGNAL("didSomething"), "important", "information")
    #信号槽
    def update_label(self, value1, value2): 
        self.textBrowser.setText(value1 + " " + value2)
    def textLableshowText(self):
        self.textBrowser.setText(_translate("MainWindow","", None))
        


if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    MainWindow = QtGui.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())

