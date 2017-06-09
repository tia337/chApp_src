#include "logindialog.h"
#include "ui_logindialog.h"
#include "ui_chatwindow.h"
#include "gui.h"

LoginDialog::LoginDialog(ChatWindow* chatPointer,Gui* guiPointer) :
    QDialog(nullptr),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    ui->inputServer->setPlaceholderText(tr("Example: name.server.com"));
    ui->inputName->setPlaceholderText(tr("Example: Name (max 16 characters)"));
    ui->inputName->setInputMask("NNNNNNNNNNNNNNNNN");
    mainWindow = chatPointer;
    chatGui = guiPointer;
}

//------------------------------------------------------------------

LoginDialog::~LoginDialog()
{
    delete ui;
}

//------------------------------------------------------------------

void LoginDialog::connected()
{
    mainWindow->setName(ui->inputName->text());
    mainWindow->setServer("Server: " + ui->inputServer->text());
    mainWindow->clearHistory();
    this->close();
}

//------------------------------------------------------------------

void LoginDialog::userNameTaken()
{
    QSound::play(":/sounds/wav/set.wav");
    ui->errorMessage->setText(tr("That name is taken"));
    ui->inputName->clear();
}

//------------------------------------------------------------------

void LoginDialog::noConnection()
{
    QSound::play(":/sounds/wav/set.wav");
    ui->errorMessage->setText(tr("Server timout. The server is down or invalid address!"));
}

//------------------------------------------------------------------

void LoginDialog::disconnectedFromServer(){
    ui->errorMessage->setText(tr("Disconnected from server!"));
    QSound::play(":/sounds/wav/set.wav");
}


//------------------------------SLOTS------------------------------
void LoginDialog::on_buttonBox_rejected()
{
    this->close();
    mainWindow->close();
}

//------------------------------------------------------------------

void LoginDialog::on_buttonBox_accepted()
{   ui->errorMessage->setText("");
    if (ui->inputName->text()=="User" or not(ui->inputName->isModified())){
        ui->errorMessage->setText(tr("Name not allowed"));
         QSound::play(":/sounds/wav/set.wav");
    }
    else if (ui->inputServer->text().contains(" ") || ui->inputServer->text()==""){

        ui->errorMessage->setText(tr("Invalid server name"));
         QSound::play(":/sounds/wav/set.wav");
    }
    else{
        chatGui->createClient(ui->inputName->text(),ui->inputServer->text());
    }
}

//-------------------------------------------------------------------
