import sys
import io
import serial
import folium # pip install folium
from PyQt5.QtWidgets import QApplication, QWidget, QHBoxLayout, QVBoxLayout
from PyQt5.QtCore import QTimer, QTime
from PyQt5.QtWebEngineWidgets import QWebEngineView # pip install PyQtWebEngine

"""
Folium in PyQt5
"""
class MyApp(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('Folium in PyQt Example')
        self.window_width, self.window_height = 1600, 1200
        self.setMinimumSize(self.window_width, self.window_height)
        timer = QTimer(self)
        timer.timeout.connect(self.gps)
        timer.start(100)
        
        global m
        
        layout = QVBoxLayout()
        self.setLayout(layout)
        
        lokasi_map = [-7.278899   , 112.793550]
        coordinates = [
            [  -7.278899   , 112.793570],
            [  -7.278982   , 112.793590],
            [  -7.279010   , 112.793750],
            [  -7.278818   , 112.793790],
            [  -7.278737   , 112.793550]]
        
        m = folium.Map(location=lokasi_map, zoom_start=100)
        #my_PolyLine=folium.PolyLine(locations=coordinates,weight=5)
        folium.PolyLine(coordinates,weight=5).add_to(m)
        
        # for each in coordinates:
            # folium.Marker(each,popup=folium.Popup(max_width=50)).add_to(m)
        
        # save map data to data object
        data = io.BytesIO()
        m.save(data, close_file=False)

        webView = QWebEngineView()
        webView.setHtml(data.getvalue().decode())
        layout.addWidget(webView)
        
        
    def gps(self):
        global m
        ser = serial.Serial('COM3', 9600, timeout=0.1)
        #line = ser.readline();
        posisi_kapal = [-7.278899   , 112.793550]
        line = 'Latitude= -7.232323 Longitude= 112.797979'
        if line:
            print(line)
            word = line.split()
            print(word)
            posisi = [float(word[1]), float(word[3])]
            print(posisi)
            folium.Marker(posisi,icon=folium.Icon(color="green", icon="ship", prefix="fa")).add_to(m)
        


if __name__ == '__main__':
    app = QApplication(sys.argv)
    app.setStyleSheet('''
        QWidget {
            font-size: 35px;
        }
    ''')
    
    myApp = MyApp()
    myApp.show()

    try:
        sys.exit(app.exec_())
    except SystemExit:
        print('Closing Window...')