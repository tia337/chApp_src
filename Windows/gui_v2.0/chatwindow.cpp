#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "QString"
#include "QColor"
#include "gui.h"
#include "QFileDialog"
#include "QMessageBox"
#include <QDebug>
#include <QTranslator>
#include <aboutwindow.h>


QTranslator translator;

//------------------------ChatWindow constructor----------------------------------------------------------------------
ChatWindow::ChatWindow(Gui* guiPointer) :
    QMainWindow(nullptr),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    chatGui = guiPointer;
    smiley= {happyFace,sadFace,straightFace,xdFace,oFace,astronaut,batman,grandpa,ironman,spiderman,pirate,angry,laurel};

    //Setup default theme
    ui->messageInput->setStyleSheet("white");
    ui->roomTree->setStyleSheet("white");
    ui->messageHistory->setStyleSheet("white");
    this->setStyleSheet("none");
    ui->mainToolBar->setStyleSheet("transparent");//*******************************************************************
}


//---------------------------function for receiving messages----------------------------------------------------------

void ChatWindow::receiveMessage(const QString from, const QString to, const QString message, const QString time){
    QString tempString = time;
    ui->messageHistory->moveCursor(QTextCursor::End);
    ui->messageHistory->setTextColor(Qt::black);
    ui->messageHistory->insertPlainText(tempString.remove(0,11).remove(5,7)+" | ");


    //controls for whispers
    if(to == name && from != name){
        lastWhisper=from;
        ui->messageHistory->setTextColor(Qt::magenta);
        ui->messageHistory->insertPlainText(from +tr(" whispers to you: "));
        ui->messageHistory->setTextColor(Qt::black);

    }
    else if (from == name && to != "root" && to !=name){
        ui->messageHistory->setTextColor(Qt::magenta);
        ui->messageHistory->insertPlainText(tr("You whisper to ") + to + ": ");
        ui->messageHistory->setTextColor(Qt::black);
    }

    else if (from == name && to == tr("root")){
        ui->messageHistory->setTextColor(Qt::blue);
        ui->messageHistory->insertPlainText(tr("You say: "));
        ui->messageHistory->setTextColor(Qt::black);
    }

    else {
        ui->messageHistory->setTextColor(Qt::blue);
        ui->messageHistory->insertPlainText(from + tr(" says: "));
        ui->messageHistory->setTextColor(Qt::black);
    }

       
    //output message
    ui->messageHistory->insertHtml(smilieConvert(message));
    ui->messageHistory->insertPlainText("\n");
    ui->messageHistory->moveCursor(QTextCursor::End);
    if(ui->messageHistory->verticalScrollBar()->value() != ui->messageHistory->verticalScrollBar()->maximum())
    {

    }
    else{
        ui->messageHistory->verticalScrollBar()->setValue(ui->messageHistory->verticalScrollBar()->maximum());
    }

}

//-------------------------------------sets username---------------------------------------------------------------------

void ChatWindow::setName(QString inName){
    name = inName;
}

//sets server
void ChatWindow::setServer(QString serverName){
    server = serverName;
    ui->roomTree->setHeaderLabel(server);
}


//------------------------------updates the tree structure with users----------------------------------------------------

void ChatWindow::updateStruct(QVector<QString> treeStruct){
    ui->roomTree->clear();
    QTreeWidgetItem *treeParent;
    treeParent=addRoot(treeStruct.at(0));
    for (int i=1; i< treeStruct.size(); i++){
        if (treeStruct.at(i)=="User"){
            treeStruct.remove(i);
            while (i < treeStruct.size()){
                addLeaf(treeParent,treeStruct.at(i));
                i++;
            }
        }
        else {
            treeParent=addSubRoot(treeParent,treeStruct.at(i));
        }
    }
    ui->roomTree->expandAll();
}


//----------------------------helpfunctions for updateStruct-------------------------------------------------------------

QTreeWidgetItem* ChatWindow::addRoot(const QString rootName){
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->roomTree);
    item->setText(0,rootName);
    ui->roomTree->addTopLevelItem(item);
    return item;
}

QTreeWidgetItem* ChatWindow::addSubRoot(QTreeWidgetItem *parent,const QString subRootName){
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0,subRootName);
    parent->addChild(item);
    return item;
}

void ChatWindow::addLeaf(QTreeWidgetItem *parent,const QString leafName){
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0,leafName);
    parent->addChild(item);
}

//------------------------ChatWindow destructor--------------------------------------------------------------------------

ChatWindow::~ChatWindow()
{
    delete ui;
}

//---------------------------If sendbutton is pressed, send message-------------------------------------------------------

void ChatWindow::on_sendButton_clicked()
{
    lastMessage=ui->messageInput->text();
    sendMessage();
    ui->messageInput->clear();
    receiver="root";
    ui->sendButton->setText(tr("Send"));
//    QSound::play(":/sounds/wav/apple_sms.wav");


}

//--------------------------------------send message on return-------------------------------------------------------------

void ChatWindow::on_messageInput_returnPressed()
{
    on_sendButton_clicked();

}


//-------------------------------------Slash command control---------------------------------------------------------------

void ChatWindow::on_messageInput_textEdited(const QString &arg1)
{
    QString to      = arg1;
    QString w       = arg1;
    QString whisper = arg1;
    w.truncate(3);
    whisper.truncate(9);

    if(w == "/w " or w =="/W "){

        to.remove("/w ",Qt::CaseInsensitive);
        if(to.length() > 0 && to.endsWith(" ")){
            to.chop(1);
            receiver = to;
            ui->sendButton->setText("To " + to);
            ui->messageInput->clear();
        }
    }
    if(whisper == "/whisper " or whisper =="/Whisper "){

        to.remove("/whisper ",Qt::CaseInsensitive);
        if(to.length() > 0 && to.endsWith(" ")){
            to.chop(1);
            receiver = to;
            ui->sendButton->setText("To " + to);
            ui->messageInput->clear();
        }
    }
    if(w=="/r " or w=="/R "){
        to.remove("/r ",Qt::CaseInsensitive);
        receiver=lastWhisper;
        ui->sendButton->setText("To " + receiver);
        ui->messageInput->clear();

    }
    if(w=="/s" or w=="/S"){
        to.remove("/s",Qt::CaseInsensitive);
        ui->messageInput->clear();
    }

}

//-----------------------------function for sending a message-----------------------------------------------------------------
void ChatWindow::sendMessage(){
    if (ui->messageInput->text()==""){
        return;

    }
    else{
        chatGui->sendMessage(name,receiver,ui->messageInput->text());
    }

}

//---------------------function for receiving old messages from current room and day------------------------------------------

void ChatWindow::receiveHistory(QVector<QString> &historyVector){
    QDate time=QDate::currentDate();
    QString timeString=time.toString("yyyy-MM-dd");

    ui->messageHistory->moveCursor(QTextCursor::Start);
    ui->messageHistory->setTextColor(Qt::black);
    ui->messageHistory->setFontItalic(true);
    ui->messageHistory->insertPlainText(tr("\n------------Chat history from ") + timeString +tr("------------")+"\n\n");
    ui->messageHistory->setFontItalic(false);

    for(int i = 0;i<historyVector.size(); i+=4){
        QString tempString=historyVector.at(i+3);
        ui->messageHistory->setTextColor(Qt::black);
        ui->messageHistory->insertPlainText(tempString.remove(0,11).remove(5,7)+" | ");
        ui->messageHistory->setTextColor(Qt::blue);
        ui->messageHistory->insertPlainText(historyVector.at(i) + tr(" says: "));
        ui->messageHistory->setTextColor(Qt::black);
        ui->messageHistory->insertHtml(smilieConvert(historyVector.at(i+2)));
        ui->messageHistory->insertPlainText("\n");
        ui->messageHistory->moveCursor(QTextCursor::End);
    }
}

//--------------function for receiving old messages from current room and past days--------------------------------------------

void ChatWindow::receiveOldHistory(QVector<QString> &historyVector){
    QDate time=QDate::currentDate();
    int i = 0 - chatGui->historyCounter;
    time =time.addDays(i+1);
    QString timeString=time.toString("yyyy-MM-dd");

    ui->messageHistory->moveCursor(QTextCursor::Start);

    for(int i = 0;i<historyVector.size(); i+=4){
        QString tempString=historyVector.at(i+3);
        ui->messageHistory->setTextColor(Qt::black);
        ui->messageHistory->insertPlainText(tempString.remove(0,11).remove(5,7)+" | ");
        ui->messageHistory->setTextColor(Qt::blue);
        ui->messageHistory->insertPlainText(historyVector.at(i) + tr(" says: "));
        ui->messageHistory->setTextColor(Qt::black);
        ui->messageHistory->insertHtml(smilieConvert(historyVector.at(i+2)));
        ui->messageHistory->insertPlainText("\n");

    }
    ui->messageHistory->moveCursor(QTextCursor::Start);
    ui->messageHistory->setTextColor(Qt::black);
    ui->messageHistory->setFontItalic(true);
    ui->messageHistory->insertPlainText(tr("\n------------Chat history from ") + timeString +"------------"+"\n\n");
    ui->messageHistory->setFontItalic(false);
    ui->messageHistory->moveCursor(QTextCursor::End);
    ui->messageHistory->verticalScrollBar()->setValue(ui->messageHistory->verticalScrollBar()->minimum());
}

//---------------------------------gets old messages--------------------------------------------------------------------------

void ChatWindow::getHistory(){
    chatGui->getHistory();
}

//---------------------------------clears messagehistory----------------------------------------------------------------------

void ChatWindow::clearHistory(){
    ui->messageHistory->clear();
}


//-------------------------------sets the smileysize---------------------------------------------------------------------------

void ChatWindow::setSmileySize(int size){
    //qDebug()<< "tempString3";
    QString sizeString = QString::number(size);
    for (int i=0; i < smiley.size(); i++ ){
        //qDebug()<< smiley.size();
        QString tempString = smiley.at(i);
        tempString.chop(23);
        tempString += " width='";
        tempString += sizeString;
        tempString += "' height='" ;
        tempString +=  sizeString;
        tempString += "'>";
        //qDebug()<< tempString;
        smiley.replace(i,tempString);
        //qDebug()<< "tempString";

    }
}

//---------------------------------Customisation menu (Themes)-------------------------------------------------------------------

void ChatWindow::on_actionDark_triggered()
{
    ui->messageInput->setStyleSheet("background-color: #c5c0ff;");
    ui->roomTree->setStyleSheet("background-color: #8ba4c5;");
    ui->messageHistory->setStyleSheet("background-color: #8ba4c5;");
    this->setStyleSheet("background-color: #152638;");
    ui->mainToolBar->setStyleSheet("background-color: #999999;");
    ui->menuBar->setStyleSheet("background-color: #999999;");
    ui->sendButton->setStyleSheet("background-color: #8ba4c5;");
}

void ChatWindow::on_actionDefault_triggered()
{
    ui->messageInput->setStyleSheet("white");
    ui->roomTree->setStyleSheet("white");
    ui->messageHistory->setStyleSheet("white");
    this->setStyleSheet("none");
    ui->mainToolBar->setStyleSheet("none");
    ui->menuBar->setStyleSheet("background-color: none;");
    ui->sendButton->setStyleSheet("background-color: none;");
}

void ChatWindow::on_actionchApp_triggered()
{
    ui->messageInput->setStyleSheet("background-color: #daeefd;");
    ui->roomTree->setStyleSheet("background-color: #daeefd;");
    ui->messageHistory->setStyleSheet("background-color: #daeefd;");
    this->setStyleSheet("background-color: #a086c3;");
    ui->mainToolBar->setStyleSheet("background-color: #b7afc9;");
    ui->menuBar->setStyleSheet("background-color: #a096ba;");
    ui->sendButton->setStyleSheet("background-color: #b7c6d4;");
}

void ChatWindow::on_actionMonokai_triggered()
{
    ui->messageInput->setStyleSheet("background-color:  #ffefdb;");
    ui->roomTree->setStyleSheet("background-color:  #ffefdb;");
    ui->messageHistory->setStyleSheet("background-color: #ffefdb;");
    this->setStyleSheet("background-color:  #ffa830;");
    ui->mainToolBar->setStyleSheet("background-color: #ffc782;");
    ui->menuBar->setStyleSheet("background-color: #ffd9aa;");
    ui->sendButton->setStyleSheet("background-color: #ffd9aa;");
}

void ChatWindow::on_actionRadiance_triggered()
{
    ui->messageInput->setStyleSheet("background-color: #ffe7d8;");
    ui->roomTree->setStyleSheet("background-color: #ffe7d8;");
    ui->messageHistory->setStyleSheet("background-color: #ffe7d8;");
    this->setStyleSheet("background-color: #916576;");
    ui->mainToolBar->setStyleSheet("background-color: #d4777f;");
    ui->menuBar->setStyleSheet("background-color: #edbeb8;");
    ui->sendButton->setStyleSheet("background-color: #edbeb8;");
}
void ChatWindow::on_actionLight_triggered()
{
    ui->messageInput->setStyleSheet("background-color: #e1f7e9;");
    ui->roomTree->setStyleSheet("background-color: #e1f7c9;");
    ui->messageHistory->setStyleSheet("background-color: #e1f7c9;");
    this->setStyleSheet("background-color: #e1f7a9;");
    ui->mainToolBar->setStyleSheet("background-color:  	#e1f7b9;");
    ui->menuBar->setStyleSheet("background-color: #e1f7e9;");
    ui->sendButton->setStyleSheet("background-color: #e1f7b9;");
}

void ChatWindow::on_actionImage_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "/home", tr("JPEG (*.jpg *.jpeg);;PNG (*.png)"));
    QString filePath(fileName);
    qDebug () << filePath;
    setStyleSheet("ChatWindow{background:url(" + filePath + ") 0 0 0 0 stretch stretch;}");
    ui->messageHistory->setStyleSheet("background-color: white;");
    QGraphicsOpacityEffect * effect1 = new QGraphicsOpacityEffect(ui->messageHistory);
    effect1->setOpacity(0.8);
    ui->messageHistory->setGraphicsEffect(effect1);
    ui->roomTree->setStyleSheet("background-color: white;");
    QGraphicsOpacityEffect * effect2 = new QGraphicsOpacityEffect(ui->roomTree);
    effect2->setOpacity(0.8);
    ui->roomTree->setGraphicsEffect(effect2);
    QGraphicsOpacityEffect * effect3 = new QGraphicsOpacityEffect(ui->messageInput);
    effect3->setOpacity(0.8);
    ui->messageInput->setGraphicsEffect(effect3);
    ui->sendButton->setStyleSheet("background-color: none;");
}

//-----------------------sets the klicked name to message reciever----------------------------------------------------------

void ChatWindow::on_roomTree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    receiver = item->text(column);
    ui->sendButton->setText(tr("To ") + receiver);
    ui->messageInput->setFocus();
    ui->messageInput->setCursorPosition(0);
}

//-----------------------translates text into a smiley-----------------------------------------------------------------------

QString ChatWindow::smilieConvert(const QString inMessage){
    QString messageConv = inMessage.toHtmlEscaped();
    messageConv.replace(":)",smiley.at(0));
    messageConv.replace(":(",smiley.at(1));
    messageConv.replace(":|",smiley.at(2));
    messageConv.replace(":o",smiley.at(3));
    messageConv.replace("xD",smiley.at(4));
    messageConv.replace(tr("astronaut"),smiley.at(5));
    messageConv.replace(tr("batman"),smiley.at(6));
    messageConv.replace(tr("grandpa"),smiley.at(7));
    messageConv.replace(tr("ironman"),smiley.at(8));
    messageConv.replace(tr("spiderman"),smiley.at(9));
    messageConv.replace(tr("pirate"),smiley.at(10));
    messageConv.replace(":@",smiley.at(11));
    messageConv.replace("unknown",smiley.at(12));

    return messageConv;
}

//-------------Change font menu------------------------------------------------------------------------------------------------

void ChatWindow::on_action13_triggered()
{
    QFont f("Dutismo Regular", 13);
    ui->messageHistory->setFont(f);
    ui->messageHistory->verticalScrollBar()->setValue(ui->messageHistory->verticalScrollBar()->maximum());
}

void ChatWindow::on_action15_triggered()
{
    QFont f("Dutismo Regular", 15);
    ui->messageHistory->setFont(f);
    ui->messageHistory->verticalScrollBar()->setValue(ui->messageHistory->verticalScrollBar()->maximum());
}

void ChatWindow::on_action20_triggered()
{
    QFont f("Dutismo Regular", 20);
    ui->messageHistory->setFont(f);
    ui->messageHistory->verticalScrollBar()->setValue(ui->messageHistory->verticalScrollBar()->maximum());
}

void ChatWindow::on_action25_triggered()
{
    QFont f("Dutismo Regular", 25);
    ui->messageHistory->setFont(f);
    ui->messageHistory->verticalScrollBar()->setValue(ui->messageHistory->verticalScrollBar()->maximum());
}



//--------------------------------Smiley size menu------------------------------------------------------------------------------

void ChatWindow::on_action13_2_triggered()
{
    setSmileySize(13);

}

void ChatWindow::on_action15_2_triggered()
{
    setSmileySize(15);

}

void ChatWindow::on_action20_2_triggered()
{
    setSmileySize(20);

}

void ChatWindow::on_action25_2_triggered()
{
    setSmileySize(25);

}

//---------------------------button trigger for getting old messages---------------------------------------------------------------

void ChatWindow::on_actionLoad_history_triggered()
{
    chatGui->getHistory();
}

//----------------------------sets messagesinput to last sent message--------------------------------------------------------------

void ChatWindow::on_actionPrevious_message_triggered()
{
    ui->messageInput->setFocus();
    ui->messageInput->clear();
    ui->messageInput->insert(lastMessage);
}

//---------------------------button trigger for translating------------------------------------------------------------------------
void ChatWindow::on_actionRussian_triggered()
{
    translator.load(":/Translations/chApp_ru.qm");
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
}

void ChatWindow::on_actionEnglish_triggered()
{
    translator.load(":/Translations/chApp_en.qm");
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
}

void ChatWindow::on_actionUkrainian_triggered()
{
    translator.load(":/Translations/chApp_ua.qm");
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
}

//---------------------------button trigger for About------------------------------------------------------------------------------

void ChatWindow::on_actionAbout_triggered(){
    AboutWindow about;
    about.setModal(true);
    about.exec();
}
