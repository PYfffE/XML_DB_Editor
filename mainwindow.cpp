#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QVector>
#include <QXmlStreamReader>
#include <QString>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("XML editor");
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

void writing(QString path, QVector<Items> outVector) {
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QXmlStreamWriter outXml(&file);
    outXml.setAutoFormatting(true);

    outXml.writeStartElement("items");
    int l = outVector.length();
    for(int i = 0; i < l; i++){


        outXml.writeStartElement("item");
        outXml.writeTextElement("id", QString::number(outVector[i].id));
        outXml.writeTextElement("product_name", outVector[i].productName);
        outXml.writeTextElement("price", QString::number(outVector[i].price));
        outXml.writeTextElement("remain", QString::number((int)(outVector[i].remain)));
        outXml.writeEndElement();

    }
    outXml.writeEndElement();

    file.close();
}

QString openFilePath = "";

void MainWindow::on_actionOpen_triggered()
{

    openFilePath = QFileDialog::getOpenFileName(this, "Open", ".//");

    for(int i = ui->tableWidget->rowCount() - 1; i >=0; i--){
        ui->tableWidget->removeRow(i);
    }

    if(openFilePath == "") return;

    QVector<Items> items = reading(openFilePath);
    int length = ui->tableWidget->rowCount();
    for(int i = 0; i < items.length(); i++){
        ui->tableWidget->insertRow(length + i);
        ui->tableWidget->setItem(i + length, 0,new QTableWidgetItem(QString::number(items[i].id)));
        ui->tableWidget->setItem(i + length, 1,new QTableWidgetItem(items[i].productName));
        ui->tableWidget->setItem(i + length, 2,new QTableWidgetItem(QString::number(items[i].price)));
        ui->tableWidget->setItem(i + length, 3,new QTableWidgetItem(QString::number(items[i].remain)));
    }

}

void MainWindow::on_pushButton_clicked()
{
    if(openFilePath == "") return;
    QVector<Items> items;

    int id;
    QString productName;
    double price;
    int remain;

    int tableLength = ui->tableWidget->rowCount();
    for(int i = 0; i < tableLength; i++){
        id = ui->tableWidget->item(i,0)->text().toInt();
        productName = ui->tableWidget->item(i,1)->text();
        price = ui->tableWidget->item(i,2)->text().toDouble();
        remain = ui->tableWidget->item(i,3)->text().toInt();
        items.append(Items(id, productName, price, remain));
    }

    writing(openFilePath, items);

}


void MainWindow::on_actionSaveAs_triggered()
{
    openFilePath = QFileDialog::getSaveFileName(this, "Save As", ".//");
    if(openFilePath == "") return;
    MainWindow::on_pushButton_clicked();
}


void MainWindow::on_actionExit_triggered() {exit(0);}


void MainWindow::on_addButton_clicked()
{
    int length = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(length);
    ui->tableWidget->setItem(length, 0,new QTableWidgetItem("null"));
    ui->tableWidget->setItem(length, 1,new QTableWidgetItem("null"));
    ui->tableWidget->setItem(length, 2,new QTableWidgetItem("null"));
    ui->tableWidget->setItem(length, 3,new QTableWidgetItem("null"));
}


void MainWindow::on_removeButton_clicked()
{
    if(ui->lineNumberEdit->text() == "") return;

    ui->tableWidget->removeRow(ui->lineNumberEdit->text().toInt() - 1);
}


void MainWindow::on_actionAbout_program_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About");
    msgBox.setText("This is a simple XML-like database editor\n"
                    "Author: PYfffE\n"
                    "Github: github.com/PYfffE\n"
                    "License: GPLv3");
    msgBox.exec();
}

