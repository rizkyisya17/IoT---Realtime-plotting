import sys
import io
import folium # pip install folium
from PyQt5.QtWidgets import QApplication, QWidget, QHBoxLayout, QVBoxLayout
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

        layout = QVBoxLayout()
        self.setLayout(layout)

        #coordinate = (-7.292238, 112.804009)
        # m = folium.Map(
        	# location=coordinate,
            # popup = 'Falcon Infomatic'
        # )
        # tooltip = "Click Here For More Info"
        # marker = folium.Marker(
            # location=coordinates,
            # popup="<stong>Allianz Arena</stong>",
            # tooltip=tooltip)
        # marker.add_to(m)
        
        coordinates = [
        [  -7.278899   , 112.793570],
        [  -7.278982   , 112.793590],
        [  -7.279010   , 112.793750],
        [  -7.278818   , 112.793790],
        [  -7.278737   , 112.793550]]
       
        posisi_kapal = [-7.278899   , 112.793550]
        
        m = folium.Map(location=posisi_kapal, zoom_start=100)
        #my_PolyLine=folium.PolyLine(locations=coordinates,weight=5)
        folium.PolyLine(coordinates,weight=5).add_to(m)
        #folium.Marker(posisi_kapal,icon=folium.Icon(color="green", icon="ship", prefix="fa")).add_to(m)
        #m.add_child(my_PolyLine)
        #m.add_to(my_kapal)
        
        
        for each in coordinates:
            folium.Marker(each,popup=folium.Popup(max_width=50)).add_to(m)
        

        # save map data to data object
        data = io.BytesIO()
        m.save(data, close_file=False)

        webView = QWebEngineView()
        webView.setHtml(data.getvalue().decode())
        layout.addWidget(webView)


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