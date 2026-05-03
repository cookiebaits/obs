#include "OBSMultiStreamDock.hpp"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <util/bmem.h>

MultiStreamDestDialog::MultiStreamDestDialog(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle("Add Multi-Stream Destination");

	QFormLayout *layout = new QFormLayout(this);

	nameEdit = new QLineEdit(this);
	serverEdit = new QLineEdit(this);
	keyEdit = new QLineEdit(this);
	keyEdit->setEchoMode(QLineEdit::Password);
	usernameEdit = new QLineEdit(this);
	passwordEdit = new QLineEdit(this);
	passwordEdit->setEchoMode(QLineEdit::Password);

	layout->addRow("Name:", nameEdit);
	layout->addRow("RTMP Server URL:", serverEdit);
	layout->addRow("Stream Key:", keyEdit);
	layout->addRow("Username (Optional):", usernameEdit);
	layout->addRow("Password (Optional):", passwordEdit);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(
		QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
		Qt::Horizontal, this);
	layout->addRow(buttonBox);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

QString MultiStreamDestDialog::GetName() const { return nameEdit->text(); }
QString MultiStreamDestDialog::GetServer() const { return serverEdit->text(); }
QString MultiStreamDestDialog::GetKey() const { return keyEdit->text(); }
QString MultiStreamDestDialog::GetUsername() const { return usernameEdit->text(); }
QString MultiStreamDestDialog::GetPassword() const { return passwordEdit->text(); }

OBSMultiStreamDock::OBSMultiStreamDock(QWidget *parent)
	: OBSDock("Multi-Stream", parent)
{
	setObjectName("MultiStreamDock");

	QWidget *content = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout(content);

	destinationList = new QListWidget(this);
	layout->addWidget(destinationList);

	QHBoxLayout *btnLayout = new QHBoxLayout();
	addBtn = new QPushButton("Add Destination", this);
	removeBtn = new QPushButton("Remove", this);
	btnLayout->addWidget(addBtn);
	btnLayout->addWidget(removeBtn);
	layout->addLayout(btnLayout);

	QHBoxLayout *streamBtnLayout = new QHBoxLayout();
	startBtn = new QPushButton("Start All", this);
	stopBtn = new QPushButton("Stop All", this);
	streamBtnLayout->addWidget(startBtn);
	streamBtnLayout->addWidget(stopBtn);
	layout->addLayout(streamBtnLayout);

	setWidget(content);

	connect(addBtn, &QPushButton::clicked, this, &OBSMultiStreamDock::AddDestination);
	connect(removeBtn, &QPushButton::clicked, this, &OBSMultiStreamDock::RemoveDestination);
	connect(startBtn, &QPushButton::clicked, this, &OBSMultiStreamDock::StartStream);
	connect(stopBtn, &QPushButton::clicked, this, &OBSMultiStreamDock::StopStream);
}

OBSMultiStreamDock::~OBSMultiStreamDock()
{
	for (auto &stream : streams) {
		obs_output_stop(stream.output);
		obs_output_release(stream.output);
		obs_service_release(stream.service);
	}
}

void OBSMultiStreamDock::AddDestination()
{
	MultiStreamDestDialog dlg(this);
	if (dlg.exec() == QDialog::Accepted) {
		QString name = dlg.GetName();
		QString server = dlg.GetServer();
		QString key = dlg.GetKey();
		QString username = dlg.GetUsername();
		QString password = dlg.GetPassword();

		if (!name.isEmpty() && !server.isEmpty()) {
			destinationList->addItem(name + " (" + server + ")");

			obs_data_t *settings = obs_data_create();
			obs_data_set_string(settings, "server", server.toUtf8().constData());
			obs_data_set_string(settings, "key", key.toUtf8().constData());
			if (!username.isEmpty()) {
				obs_data_set_string(settings, "username", username.toUtf8().constData());
				obs_data_set_string(settings, "password", password.toUtf8().constData());
			}

			obs_service_t *service = obs_service_create("rtmp_custom", "multistream_service", settings, nullptr);
			obs_data_release(settings);

			obs_output_t *output = obs_output_create("rtmp_output", "multistream_output", nullptr, nullptr);
			obs_output_set_service(output, service);

			streams.push_back({output, service, name});
		}
	}
}

void OBSMultiStreamDock::RemoveDestination()
{
	QList<QListWidgetItem*> items = destinationList->selectedItems();
	for (auto item : items) {
		int row = destinationList->row(item);
		if (row >= 0 && (size_t)row < streams.size()) {
			obs_output_release(streams[row].output);
			obs_service_release(streams[row].service);
			streams.erase(streams.begin() + row);
		}
		delete destinationList->takeItem(row);
	}
}

void OBSMultiStreamDock::StartStream()
{
	for (auto &stream : streams) {
		obs_output_start(stream.output);
	}
}

void OBSMultiStreamDock::StopStream()
{
	for (auto &stream : streams) {
		obs_output_stop(stream.output);
	}
}
