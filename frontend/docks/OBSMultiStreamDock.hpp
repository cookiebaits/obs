#pragma once

#include "OBSDock.hpp"
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <obs.h>

class MultiStreamDestDialog : public QDialog {
	Q_OBJECT

public:
	explicit MultiStreamDestDialog(QWidget *parent = nullptr);

	QString GetName() const;
	QString GetServer() const;
	QString GetKey() const;
	QString GetUsername() const;
	QString GetPassword() const;

private:
	QLineEdit *nameEdit;
	QLineEdit *serverEdit;
	QLineEdit *keyEdit;
	QLineEdit *usernameEdit;
	QLineEdit *passwordEdit;
};

class OBSMultiStreamDock : public OBSDock {
	Q_OBJECT

public:
	explicit OBSMultiStreamDock(QWidget *parent = nullptr);
	~OBSMultiStreamDock();

private slots:
	void AddDestination();
	void RemoveDestination();
	void StartStream();
	void StopStream();

private:
	QListWidget *destinationList;
	QPushButton *addBtn;
	QPushButton *removeBtn;
	QPushButton *startBtn;
	QPushButton *stopBtn;

	struct MultiStream {
		obs_output_t *output;
		obs_service_t *service;
		QString name;
	};

	std::vector<MultiStream> streams;
};
