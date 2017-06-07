#include "vec3.h"

#include <QtWidgets/QHBoxLayout>

namespace properties {

vec3_ui::vec3_ui() {
	m_widget = new QWidget(NULL);

	QHBoxLayout* layout = new QHBoxLayout(m_widget);
	layout->setContentsMargins(0,0,0,0);

	for(unsigned a=0;a<3;++a) {
		m_values.push_back(new QDoubleSpinBox(NULL));
		layout->addWidget(m_values.back());

		m_connections.push_back(QObject::connect(
			m_values.back(),
			static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
			[this]() -> void {
				callValueChangedCallbacks();
			}
		));

		m_values.back()->setKeyboardTracking(false);
		m_values.back()->setRange(-1e13, 1e13);
	}
}

vec3_ui::~vec3_ui() {
	for(auto& c : m_connections)
		QObject::disconnect(c);
}

Imath::Vec3<float> vec3_ui::get() const {
	Imath::Vec3<float> result;
	for(unsigned a=0;a<3;++a)
		result[a] = m_values[a]->value();

	return result;
}

void vec3_ui::set(const Imath::Vec3<float>& value) {
	for(unsigned a=0;a<3;++a)
		if(m_values[a]->value() != value[a])
			m_values[a]->setValue(value[a]);
}

QWidget* vec3_ui::widget() {
	return m_widget;
}

void vec3_ui::onFlagsChanged(unsigned flags) {
	for(unsigned a=0;a<3;++a) {
		m_values[a]->setReadOnly(flags & kOutput);
		m_values[a]->setDisabled((flags & kDirty) || (flags & kDisabled));
	}
}

}

