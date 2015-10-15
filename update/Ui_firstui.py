# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'C:\Users\darney\New Folder\update\firstui.ui'
#
# Created by: PyQt4 UI code generator 4.11.4
#
# WARNING! All changes made in this file will be lost!
import random
from PyQt4 import QtCore, QtGui, QtNetwork

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
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.resize(400, 300)
        self.centralWidget = QtGui.QWidget(MainWindow)
        self.centralWidget.setObjectName(_fromUtf8("centralWidget"))
        self.textBrowser = QtGui.QTextBrowser(self.centralWidget)
        self.textBrowser.setGeometry(QtCore.QRect(90, 10, 300,200))
        self.textBrowser.setObjectName(_fromUtf8("textBrowser"))
        self.pushButton = QtGui.QPushButton(self.centralWidget)
        self.pushButton.setGeometry(QtCore.QRect(10, 10, 75, 23))
        self.pushButton.setObjectName(_fromUtf8("pushButton"))

        self.sendButton = QtGui.QPushButton(self.centralWidget)
        self.sendButton.setGeometry(QtCore.QRect(10, 40, 75, 23))
        self.sendButton.setObjectName(_fromUtf8("SendButton"))
        
        self.openFileNameButton = QtGui.QPushButton(self.centralWidget)
        self.openFileNameButton.setGeometry(QtCore.QRect(10, 70, 75, 23))
        self.openFileNameButton.setObjectName(_fromUtf8("openFilebutton"))
        
        self.native = QtGui.QCheckBox()
        self.native.setText("Use native file dialog.")
        self.native.setChecked(True)
        
        
        MainWindow.setCentralWidget(self.centralWidget)
        self.tcp=QtNetwork.QTcpServer()
        self.tcp.listen(QtNetwork.QHostAddress.Any, 9001);
        #self.tcp.waitForNewConnection(-1)

        self.retranslateUi(MainWindow)
        #self.tcp.newConnection.connect(self.newConnect)
        #打开文件获取文件名
        self.openFileNameButton.clicked.connect(self.setOpenFileName)
        
        self.fortunes = (
                "You've been leading a dog's life. Stay off the furniture.",
                "You've got to think about tomorrow.",
                "You will be surprised by a loud noise.",
                "You will feel hungry again in another hour.",
                "You might have mail.",
                "You cannot kill time without injuring eternity.",
                "Computers are not intelligent. They only think they are.")
        
        QtCore.QObject.connect(self.tcp, QtCore.SIGNAL(_fromUtf8("newConnection()")), self.newConnect)
        #QtCore.QObject.connect(self.textBrowser, QtCore.SIGNAL("didSomething"), self.update_label) 
        QtCore.QObject.connect(self.pushButton, QtCore.SIGNAL(_fromUtf8("clicked()")), self.textLableshowText)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)
        self.do_something()
    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow", None))
        self.pushButton.setText(_translate("MainWindow", "清空", None))
        self.sendButton.setText(_translate("MainWindow", "发送", None))
        self.openFileNameButton.setText(_translate("MainWindow", "打开文件", None))
    def setOpenFileName(self):    
        options = QtGui.QFileDialog.Options()
        if not self.native.isChecked():
            options |= QtGui.QFileDialog.DontUseNativeDialog
        fileName = QtGui.QFileDialog.getOpenFileName(self.centralWidget,
                "QFileDialog.getOpenFileName()",
                '', 
                "All Files (*);;Text Files (*.txt)", options)
        if fileName:
            print(fileName)
            self.textBrowser.setText(fileName)
#发送包一 以及 传输MD5校验值
#            self
#            sendFortune(self, socket, data)
        

        
    def newConnect(self):
        
        self.tcpsocket=self.tcp.nextPendingConnection()
        self.tcpsocket.readyRead.connect(self.readMessage)
        #self.tcpsocket.disconnected.connect(self.tcpsocket.deleteLater)
        self.tcpsocket.disconnected.connect(self.socketclose)
        print(self.tcpsocket.localAddress().toString())
        print(self.tcpsocket.localPort)
    def socketclose(self):
        self.tcpsocket.close()
        
    def readMessage(self):
        qba=self.tcpsocket.readAll()
#        cc=qba.data()
#        print(type(cc))
        qs1 = str(qba, encoding = "utf-8")
        
        print(type(qs1))
        self.textBrowser.append(_translate(qs1, qs1, None))
        self.sendFortune(self.tcpsocket, '')
        
        
    def sendFortune(self, socket, data):
        block = QtCore.QByteArray()
        out = QtCore.QDataStream(block, QtCore.QIODevice.WriteOnly)
        #out.setVersion(QtCore.QDataStream.Qt_4_0)
        #out.writeUInt16(0)
        fortune = self.fortunes[random.randint(0, len(self.fortunes) - 1)]
        try:
            # Python v3.
            fortune = bytes(fortune, encoding='ascii')
        except:
            # Python v2.
            pass
        print(fortune)
        out.writeString(fortune)
        out.device().seek(4)
        #out.writeUInt16(block.size() - 2)
        print(block)
        
#b = ''.join(['x','y','z'])
#        clientConnection = self.tcpServer.nextPendingConnection()
        
        try:
            socket.write(fortune)
        except:
            print("error")
            
        #self.tcpsocket.disconnectFromHost()
    
    def do_something(self): 
        QtCore.QObject.emit(self.textBrowser,QtCore.SIGNAL("didSomething"), "important", "information")

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

