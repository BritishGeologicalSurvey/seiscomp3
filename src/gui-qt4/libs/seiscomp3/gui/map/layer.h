/***************************************************************************
 *   Copyright (C) by GFZ Potsdam                                          *
 *                                                                         *
 *   You can redistribute and/or modify this program under the             *
 *   terms of the SeisComP Public License.                                 *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   SeisComP Public License for more details.                             *
 ***************************************************************************/

#ifndef __SEISCOMP_GUI_MAP_LAYER_H__
#define __SEISCOMP_GUI_MAP_LAYER_H__

#ifndef Q_MOC_RUN
#include <seiscomp3/core/rtti.h>

#include <seiscomp3/core/baseobject.h>
#include <seiscomp3/core/interfacefactory.h>
#endif

#include <seiscomp3/gui/qt4.h>
#include <seiscomp3/gui/map/legend.h>
#include <seiscomp3/gui/map/mapsymbol.h>

#include <QObject>

class QContextMenuEvent;
class QDialog;
class QMenu;
class QPainter;
class QWidget;

namespace Seiscomp {

namespace Config {
class Config;
}

namespace Gui {
namespace Map {

class Canvas;

DEFINE_SMARTPOINTER(Layer);

class SC_GUI_API Layer : public QObject, public Seiscomp::Core::BaseObject {
	DECLARE_RTTI;
	Q_OBJECT

	public:
		enum UpdateHint{Position};
		Q_DECLARE_FLAGS(UpdateHints, UpdateHint)

		typedef QList<Legend*> Legends;

	public:
		Layer(QObject* parent = NULL);
		virtual ~Layer();

	public:
		virtual void setConfig(const std::string &/*config*/) {}
		virtual void init(const Seiscomp::Config::Config&);
		virtual void draw(const Seiscomp::Gui::Map::Canvas*, QPainter&) {}

		virtual Layer &operator =(const Layer &other);
		virtual Layer *clone() const { return NULL; }

	public:
		void setName(const QString&);
		const QString &name() const;

		void setDescription(const QString&);
		const QString &description() const;

		bool addLegend(Seiscomp::Gui::Map::Legend *legend);
		bool removeLegend(Seiscomp::Gui::Map::Legend *legend);

		int legendCount() const { return _legends.count(); }
		Legend *legend(int i) const;

		const QList<Legend*> &legends() const { return _legends; }

		virtual void setVisible(bool);
		bool isVisible() const;

		void setAntiAliasingEnabled(bool);
		bool isAntiAliasingEnabled() const;

	public:
		virtual void calculateMapPosition(const Map::Canvas *canvas);
		virtual void bufferUpdated(Map::Canvas *canvas);
		virtual bool filterContextMenuEvent(QContextMenuEvent*, QWidget*);
		virtual QMenu *menu(QWidget*) const;

	signals:
		void updateRequested(const Layer::UpdateHints& = UpdateHints());

	private slots:
		void onObjectDestroyed(QObject *object);

	private:
		QString                       _name;
		QString                       _description;
		bool                          _visible;
		bool                          _antiAliasing;
		Legends                       _legends;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Layer::UpdateHints)

DEFINE_INTERFACE_FACTORY(Layer);

#define REGISTER_LAYER_INTERFACE(Class, Service) \
Seiscomp::Core::Generic::InterfaceFactory<Seiscomp::Gui::Map::Layer, Class> __##Class##InterfaceFactory__(Service)

} // namespace Map
} // namespce Gui
} // namespace Seiscomp

#endif
