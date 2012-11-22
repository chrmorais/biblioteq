/*
** -- Qt Includes --
*/

#include <QSqlField>
#include <QSqlRecord>

/*
** Includes photograph collection-specific methods.
*/

/*
** -- Local Includes --
*/

#include "qtbook.h"
#include "qtbook_photographcollection.h"

extern qtbook *qmain;
extern QApplication *qapp;

/*
** -- qtbook_photographcollection() --
*/

qtbook_photographcollection::qtbook_photographcollection
(QMainWindow *parentArg,
 const QString &oidArg,
 const int rowArg):QMainWindow()
{
  QMenu *menu = 0;
  QGraphicsScene *scene1 = 0;
  QGraphicsScene *scene2 = 0;

  if((menu = new(std::nothrow) QMenu(this)) == 0)
    qtbook::quit("Memory allocation failure", __FILE__, __LINE__);

  if((scene1 = new(std::nothrow) QGraphicsScene(this)) == 0)
    qtbook::quit("Memory allocation failure", __FILE__, __LINE__);

  if((scene2 = new(std::nothrow) QGraphicsScene(this)) == 0)
    qtbook::quit("Memory allocation failure", __FILE__, __LINE__);

  oid = oidArg;
  row = rowArg;
  isQueryEnabled = false;
  parentWid = parentArg;
  pc.setupUi(this);
#ifdef Q_WS_MAC
  setAttribute(Qt::WA_MacMetalStyle, true);
#endif
  updateFont(qapp->font(), static_cast<QWidget *> (this));
  connect(pc.okButton, SIGNAL(clicked(void)), this, SLOT(slotGo(void)));
  connect(pc.cancelButton, SIGNAL(clicked(void)), this,
	  SLOT(slotCancel(void)));
  connect(pc.resetButton, SIGNAL(clicked(void)), this,
	  SLOT(slotReset(void)));
  connect(pc.printButton, SIGNAL(clicked(void)), this, SLOT(slotPrint(void)));
  connect(menu->addAction(tr("Reset Collection &Image")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Collection &ID")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Collection &Title")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Collection &About")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Collection &Notes")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Item &Image")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Item &ID")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Item &Title")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Item &Creator(s)")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Item &Publication Date")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Item &Copies")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Item &Medium")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Item &Reproduction Number")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Item &Copyright")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Item &Call Number")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Item &Other Number")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Item &Location")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Item &Notes")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Item &Subjects")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  connect(menu->addAction(tr("Reset Item &Format")),
	  SIGNAL(triggered(void)), this, SLOT(slotReset(void)));
  pc.resetButton->setMenu(menu);

  QString errorstr("");

  qapp->setOverrideCursor(Qt::WaitCursor);
  pc.location->addItems
    (misc_functions::getLocations(qmain->getDB(),
				  "Photograph Collection",
				  errorstr));
  qapp->restoreOverrideCursor();

  if(!errorstr.isEmpty())
    qmain->addError
      (QString(tr("Database Error")),
       QString(tr("Unable to retrieve the photograph collection locations.")),
       errorstr, __FILE__, __LINE__);

  pc.thumbnail_collection->setScene(scene1);
  pc.thumbnail_item->setScene(scene2);

  if(pc.location->findText(tr("UNKNOWN")) == -1)
    pc.location->addItem(tr("UNKNOWN"));

  resize(0.90 * parentWid->size().width(),
	 0.90 * parentWid->size().height());
  pc.splitter->setStretchFactor(0, 0);
  pc.splitter->setStretchFactor(1, 1);
  pc.splitter->setStretchFactor(2, 0);
  misc_functions::center(this, parentWid);
  misc_functions::hideAdminFields(this, qmain->getRoles());
}

/*
** -- ~qtbook_photographcollection() --
*/

qtbook_photographcollection::~qtbook_photographcollection()
{
}

/*
** -- slotGo() --
*/

void qtbook_photographcollection::slotGo(void)
{
}

/*
** -- search() --
*/

void qtbook_photographcollection::search
(const QString &field, const QString &value)
{
  Q_UNUSED(field);
  Q_UNUSED(value);
  pc.addItemButton->setVisible(false);
  pc.thumbnail_collection->setVisible(false);
  pc.select_image_collection->setVisible(false);
  pc.collectionGroup->setVisible(false);
  pc.itemGroup->setVisible(false);

  QList<QAction *> actions = pc.resetButton->menu()->actions();

  actions[0]->setVisible(false);
  actions.clear();
  setWindowTitle(tr("BiblioteQ: Database Photograph Collection Search"));
  engWindowTitle = "Search";
  pc.id_collection->setFocus();
  misc_functions::center(this, parentWid);
  show();
}

/*
** -- updateWindow() --
*/

void qtbook_photographcollection::updateWindow(const int state)
{
  QString str = "";

  if(state == qtbook::EDITABLE)
    {
      pc.okButton->setVisible(true);
      pc.addItemButton->setVisible(true);
      pc.resetButton->setVisible(true);
      str = QString(tr("BiblioteQ: Modify Photograph Collection Entry (")) +
	pc.id_collection->text() + tr(")");
      engWindowTitle = "Modify";
    }
  else
    {
      pc.okButton->setVisible(false);
      pc.addItemButton->setVisible(false);
      pc.resetButton->setVisible(false);
      str = QString(tr("BiblioteQ: View Photograph Collection Details (")) +
	pc.id_collection->text() + tr(")");
      engWindowTitle = "View";
    }

  setWindowTitle(str);
}

/*
** -- modify() --
*/

void qtbook_photographcollection::modify(const int state)
{
  QString str("");
  QString fieldname("");
  QString searchstr("");
  QVariant var;
  QSqlQuery query(qmain->getDB());

  if(state == qtbook::EDITABLE)
    {
      setWindowTitle(tr("BiblioteQ: Modify Photograph Collection Entry"));
      engWindowTitle = "Modify";
      pc.okButton->setVisible(true);
      pc.addItemButton->setEnabled(true);
      pc.resetButton->setVisible(true);
      pc.select_image_collection->setVisible(true);
      pc.select_image_item->setVisible(true);
      misc_functions::highlightWidget
	(pc.id_collection, QColor(255, 248, 220));
      misc_functions::highlightWidget
	(pc.title_collection, QColor(255, 248, 220));
      misc_functions::highlightWidget
	(pc.id_item, QColor(255, 248, 220));
      misc_functions::highlightWidget
	(pc.title_item, QColor(255, 248, 220));
      misc_functions::highlightWidget
	(pc.creators_item->viewport(), QColor(255, 248, 220));
      misc_functions::highlightWidget
	(pc.medium_item, QColor(255, 248, 220));
      misc_functions::highlightWidget
	(pc.reproduction_number_item->viewport(), QColor(255, 248, 220));
      misc_functions::highlightWidget
	(pc.copyright_item->viewport(), QColor(255, 248, 220));
    }
  else
    {
      setWindowTitle(tr("BiblioteQ: View Photograph Collection Details"));
      engWindowTitle = "View";
      pc.okButton->setVisible(false);
      pc.addItemButton->setEnabled(false);
      pc.resetButton->setVisible(false);
      pc.select_image_collection->setVisible(false);
      pc.select_image_item->setVisible(false);

      QList<QAction *> actions = pc.resetButton->menu()->actions();

      actions[0]->setVisible(false);
      actions.clear();
    }

  searchstr = "SELECT id, "
    "title, "
    "about, "
    "notes, "
    "image "
    "FROM "
    "photograph_collection "
    "WHERE myoid = ";
  searchstr.append(oid);
  qapp->setOverrideCursor(Qt::WaitCursor);

  if(!query.exec(searchstr) || !query.next())
    {
      qapp->restoreOverrideCursor();
      qmain->addError(QString(tr("Database Error")),
		      QString(tr("Unable to retrieve the selected photograph "
				 "collection's data.")),
		      query.lastError().text(), __FILE__, __LINE__);
      QMessageBox::critical(this, tr("BiblioteQ: Database Error"),
			    tr("Unable to retrieve the selected photograph "
			       "collection's data."));
      return;
    }
  else
    {
      qapp->restoreOverrideCursor();

      for(int i = 0; i < query.record().count(); i++)
	{
	  var = query.record().field(i).value();
	  fieldname = query.record().fieldName(i);

	  if(fieldname == "id")
	    {
	      pc.id_collection->setText(var.toString());

	      if(state == qtbook::EDITABLE)
		{
		  str = QString
		    (tr("BiblioteQ: Modify Photograph Collection Entry (")) +
		    var.toString() + tr(")");
		  engWindowTitle = "Modify";
		}
	      else
		{
		  str = QString(tr("BiblioteQ: View Photograph Collection "
				   "Details (")) +
		    var.toString() + tr(")");
		  engWindowTitle = "View";
		}

	      setWindowTitle(str);
	    }
	  else if(fieldname == "title")
	    pc.title_collection->setText(var.toString());
	  else if(fieldname == "about")
	    pc.about_collection->setPlainText(var.toString());
	  else if(fieldname == "notes")
	    pc.notes_collection->setPlainText(var.toString());
	}

      foreach(QLineEdit *textfield, findChildren<QLineEdit *>())
	textfield->setCursorPosition(0);

      storeData(this);
      showNormal();
    }

  pc.id_collection->setFocus();
  raise();
}

/*
** -- insert() --
*/

void qtbook_photographcollection::insert(void)
{
  pc.okButton->setText(tr("&Save"));
  pc.addItemButton->setEnabled(false);
  pc.publication_date->setDate(QDate::fromString("01/01/2000",
						 "MM/dd/yyyy"));
  misc_functions::highlightWidget
    (pc.id_collection, QColor(255, 248, 220));
  misc_functions::highlightWidget
    (pc.title_collection, QColor(255, 248, 220));
  misc_functions::highlightWidget
    (pc.id_item, QColor(255, 248, 220));
  misc_functions::highlightWidget
    (pc.title_item, QColor(255, 248, 220));
  misc_functions::highlightWidget
    (pc.creators_item->viewport(), QColor(255, 248, 220));
  misc_functions::highlightWidget
    (pc.medium_item, QColor(255, 248, 220));
  misc_functions::highlightWidget
    (pc.reproduction_number_item->viewport(), QColor(255, 248, 220));
  misc_functions::highlightWidget
    (pc.copyright_item->viewport(), QColor(255, 248, 220));
  setWindowTitle(tr("BiblioteQ: Create Photograph Collection Entry"));
  engWindowTitle = "Create";
  pc.id_collection->setFocus();
  storeData(this);
  show();
}

/*
** -- slotReset() --
*/

void qtbook_photographcollection::slotReset(void)
{
  QAction *action = qobject_cast<QAction *> (sender());

  if(action != 0)
    {
      QList<QAction *> actions = pc.resetButton->menu()->actions();

      if(action == actions[0])
	pc.thumbnail_collection->clear();
      else if(action == actions[1])
	{
	  pc.id_collection->clear();
	  pc.id_collection->setFocus();
	}
      else if(action == actions[2])
	{
	  pc.title_collection->clear();
	  pc.title_collection->setFocus();
	}
      else if(action == actions[3])
	{
	  pc.about_collection->clear();
	  pc.about_collection->setFocus();
	}
      else if(action == actions[4])
	{
	  pc.notes_collection->clear();
	  pc.notes_collection->setFocus();
	}
      else if(action == actions[5])
	pc.thumbnail_item->clear();
      else if(action == actions[6])
	{
	  pc.id_item->clear();
	  pc.id_item->setFocus();
	}
      else if(action == actions[7])
	{
	  pc.title_item->clear();
	  pc.title_item->setFocus();
	}
      else if(action == actions[8])
	{
	  pc.creators_item->clear();
	  pc.creators_item->setFocus();
	}
      else if(action == actions[9])
	{
	  pc.publication_date->setDate
	    (QDate::fromString("01/01/2000", "MM/dd/yyyy"));
	  pc.publication_date->setFocus();
	}
      else if(action == actions[10])
	{
	  pc.quantity->setValue(1);
	  pc.quantity->setFocus();
	}
      else if(action == actions[11])
	{
	  pc.medium_item->clear();
	  pc.medium_item->setFocus();
	}
      else if(action == actions[12])
	{
	  pc.reproduction_number_item->clear();
	  pc.reproduction_number_item->setFocus();
	}
      else if(action == actions[13])
	{
	  pc.copyright_item->clear();
	  pc.copyright_item->setFocus();
	}
      else if(action == actions[14])
	{
	  pc.call_number_item->clear();
	  pc.call_number_item->setFocus();
	}
      else if(action == actions[15])
	{
	  pc.other_number_item->clear();
	  pc.other_number_item->setFocus();
	}
      else if(action == actions[16])
	{
	  pc.location->setCurrentIndex(0);
	  pc.location->setFocus();
	}
      else if(action == actions[17])
	{
	  pc.notes_item->clear();
	  pc.notes_item->setFocus();
	}
      else if(action == actions[18])
	{
	  pc.subjects_item->clear();
	  pc.subjects_item->setFocus();
	}
      else if(action == actions[19])
	{
	  pc.format_item->clear();
	  pc.format_item->setFocus();
	}

      actions.clear();
    }
  else
    {
      /*
      ** Reset all.
      */

      pc.thumbnail_collection->clear();
      pc.id_collection->clear();
      pc.title_collection->clear();
      pc.about_collection->clear();
      pc.notes_collection->clear();
      pc.thumbnail_item->clear();
      pc.id_item->clear();
      pc.title_item->clear();
      pc.creators_item->clear();
      pc.publication_date->setDate
	(QDate::fromString("01/01/2000", "MM/dd/yyyy"));
      pc.quantity->setValue(1);
      pc.medium_item->clear();
      pc.reproduction_number_item->clear();
      pc.copyright_item->clear();
      pc.call_number_item->clear();
      pc.other_number_item->clear();
      pc.location->setCurrentIndex(0);
      pc.notes_item->clear();
      pc.subjects_item->clear();
      pc.format_item->clear();
      pc.id_collection->setFocus();
    }
}

/*
** -- closeEvent() --
*/

void qtbook_photographcollection::closeEvent(QCloseEvent *e)
{
  if(engWindowTitle.contains("Create") ||
     engWindowTitle.contains("Modify"))
    if(hasDataChanged(this))
      if(QMessageBox::question(this, tr("BiblioteQ: Question"),
			       tr("You have unsaved data. Continue closing?"),
			       QMessageBox::Yes | QMessageBox::No,
			       QMessageBox::No) == QMessageBox::No)
	{
	  if(e)
	    e->ignore();

	  return;
	}

  qmain->removePhotographCollection(this);
  QMainWindow::closeEvent(e);
}

/*
** -- slotCancel() --
*/

void qtbook_photographcollection::slotCancel(void)
{
  close();
}

/*
** -- slotQuery() --
*/

void qtbook_photographcollection::slotQuery(void)
{
}

/*
** -- slotPrint() --
*/

void qtbook_photographcollection::slotPrint(void)
{
  html = "";
  html += "<b>" + tr("Collection ID:") + "</b> " +
    pc.id_collection->text().trimmed() + "<br>";
  html += "<b>" + tr("Collection Title:") + "</b> " +
    pc.title_collection->text().trimmed() + "<br>";
  html += "<b>" + tr("About:") + "</b> " +
    pc.about_collection->toPlainText().trimmed() + "<br>";
  html += "<b>" + tr("Collection Nodes:") + "</b> " +
    pc.notes_collection->toPlainText().trimmed() + "<br>";
  html += "<b>" + tr("Item ID:") + "</b> " +
    pc.id_item->text().trimmed() + "<br>";
  html += "<b>" + tr("Item Title:") + "</b> " +
    pc.title_item->text().trimmed() + "<br>";
  html += "<b>" + tr("Item Creator(s):") + "</b> " +
    pc.creators_item->toPlainText().trimmed() + "<br>";
  html += "<b>" + tr("Item Publication Date:") + "</b> " +
    pc.publication_date->date().toString(Qt::SystemLocaleShortDate) + "<br>";
  html += "<b>" + tr("Item Copies:") + "</b> " +
    pc.quantity->text() + "<br>";
  html += "<b>" + tr("Item Medium:") + "</b> " +
    pc.medium_item->text().trimmed() + "<br>";
  html += "<b>" + tr("Item Reproduction Number:") + "</b> " +
    pc.reproduction_number_item->toPlainText().trimmed() + "<br>";
  html += "<b>" + tr("Item Copyright:") + "</b> " +
    pc.copyright_item->toPlainText().trimmed() + "<br>";
  html += "<b>" + tr("Item Call Number:") + "</b> " +
    pc.call_number_item->text().trimmed() + "<br>";
  html += "<b>" + tr("Item Other Number:") + "</b> " +
    pc.other_number_item->text().trimmed() + "<br>";
  html += "<b>" + tr("Item Location:") + "</b> " +
    pc.location->currentText().trimmed() + "<br>";
  html += "<b>" + tr("Item Notes:") + "</b> " +
    pc.notes_item->toPlainText().trimmed() + "<br>";
  html += "<b>" + tr("Item Subjects:") + "</b> " +
    pc.subjects_item->toPlainText().trimmed() + "<br>";
  html += "<b>" + tr("Item Format:") + "</b> " +
    pc.format_item->toPlainText().trimmed() + "<br>";
  print(this);
}

/*
** -- slotSelectImage() --
*/

void qtbook_photographcollection::slotSelectImage(void)
{
  QFileDialog dialog(this);
  QPushButton *button = qobject_cast<QPushButton *> (sender());

#ifdef Q_WS_MAC
  dialog.setAttribute(Qt::WA_MacMetalStyle, true);
#endif
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setDirectory(QDir::homePath());
  dialog.setNameFilter("Image Files (*.bmp *.jpg *.jpeg *.png)");

  if(button == pc.select_image_collection)
    dialog.setWindowTitle(tr("BiblioteQ: Photograph Collection "
			     "Image Selection"));
  else
    dialog.setWindowTitle(tr("BiblioteQ: Photograph Collection Item "
			     "Image Selection"));

  dialog.exec();

  if(dialog.result() == QDialog::Accepted)
    {
      if(button == pc.select_image_collection)
	{
	  pc.thumbnail_collection->clear();
	  pc.thumbnail_collection->image =
	    QImage(dialog.selectedFiles().at(0));
	  pc.thumbnail_collection->imageFormat =
	    dialog.selectedFiles().at(0).mid
	    (dialog.selectedFiles().at(0).lastIndexOf(".") + 1).toUpper();
	  pc.thumbnail_collection->scene()->addPixmap
	    (QPixmap().fromImage(pc.thumbnail_collection->image));
	  pc.thumbnail_collection->scene()->items().at(0)->setFlags
	    (QGraphicsItem::ItemIsSelectable);
	}
      else
	{
	  pc.thumbnail_item->clear();
	  pc.thumbnail_item->image = QImage(dialog.selectedFiles().at(0));
	  pc.thumbnail_item->imageFormat = dialog.selectedFiles().at(0).mid
	    (dialog.selectedFiles().at(0).lastIndexOf(".") + 1).toUpper();
	  pc.thumbnail_item->scene()->addPixmap
	    (QPixmap().fromImage(pc.thumbnail_item->image));
	  pc.thumbnail_item->scene()->items().at(0)->setFlags
	    (QGraphicsItem::ItemIsSelectable);
	}
    }
}

/*
** -- duplicate() --
*/

void qtbook_photographcollection::duplicate
(const QString &p_oid, const int state)
{
  modify(state); // Initial population.
  oid = p_oid;
  setWindowTitle(tr("BiblioteQ: Duplicate Photograph Collection Entry"));
  engWindowTitle = "Create";
}

/*
** -- changeEvent() --
*/

void qtbook_photographcollection::changeEvent(QEvent *event)
{
  if(event)
    switch(event->type())
      {
      case QEvent::LanguageChange:
	{
	  pc.retranslateUi(this);
	  break;
	}
      default:
	break;
      }

  QMainWindow::changeEvent(event);
}