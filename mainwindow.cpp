#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QVector>
#include <QXmlStreamReader>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

class Items{
public:

    int id;
    QString productName;
    double price;
    int remain;

    Items(){}

    Items(int id, QString productName, double price, int remain){
        this->id = id;
        this->productName = productName;
        this->price = price;
        this->remain = remain;
    }
};

QVector<Items> reading(QString path) {
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    int id;
    QString productName;
    double price;
    int remain;

    QVector<Items> outVector;

    QXmlStreamReader xml(&file);
    while (!xml.atEnd() && !xml.hasError())
        {
            QXmlStreamReader::TokenType token = xml.readNext();
            if (token == QXmlStreamReader::StartDocument)
                continue;
            if (token == QXmlStreamReader::StartElement)
            {
                if (xml.name() == "items")
                    continue;
                if (xml.name() == "item"){



                    xml.readNextStartElement();
                    id = xml.readElementText().toInt();

                    xml.readNextStartElement();
                    productName = xml.readElementText();

                    xml.readNextStartElement();
                    price = xml.readElementText().toDouble();


                    xml.readNextStartElement();
                    remain = xml.readElementText().toInt();

                    outVector.append(Items(id, productName, price, remain));

                }
            }
        }
    file.close();
    return outVector;
}

QString openFilePath = "";

void MainWindow::on_actionOpen_triggered()
{
    openFilePath = QFileDialog::getOpenFileName(this, "Open", "C://");

    if(openFilePath == "") return;

    QVector<Items> items = reading(openFilePath);
    int length = ui->tableWidget->rowCount();
    for(int i = 0; i < items.length(); i++){
        qDebug() << length;
        ui->tableWidget->insertRow(length + i);
        ui->tableWidget->setItem(i + length, 0,new QTableWidgetItem(QString::number(items[i].id)));
        ui->tableWidget->setItem(i + length, 1,new QTableWidgetItem(items[i].productName));
        ui->tableWidget->setItem(i + length, 2,new QTableWidgetItem(QString::number(items[i].price)));
        ui->tableWidget->setItem(i + length, 3,new QTableWidgetItem(QString::number(items[i].remain)));
        ui->tableWidget->update();
    }

}

