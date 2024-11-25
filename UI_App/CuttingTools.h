#ifndef _CUTTING_TOOLS_
#define _CUTTING_TOOLS_
#include <qobject.h>
#include <vector>
#include <controller.h>
#include <vsn_renderobject.h>
#include <qwindow.h>
#include <qt_openglwindow.h>
#include <vsn_texture2d.h>
#include <qt_resstream.h>
#include <qvalidator.h>

//класс хранящий в себе секущие плоскости используемые в виджете Vision3Dview
class CuttingTools:public QObject
{
	Q_OBJECT
private:
	QtVision::QtOpenGLSceneWindow* parent = nullptr;
	std::vector<VSN::SectionPlaneId> m_planes;
	VSN::Controller* m_controller=nullptr;
	size_t m_CountOfCutingPlanes = 0;
	size_t m_currentCuttingTool = 0;
	std::shared_ptr<Material> m_material;
public:
	CuttingTools(QtVision::QtOpenGLSceneWindow* parent = nullptr);

	// Возвращает количество секущих.
	size_t count() const;

	// Устанавливает количество секущих.
	void setCount(size_t count);

	// Проверяет, инвертирована ли секущая.
	bool IsInverted();

	// Проверяет, включена ли секущая.
	bool IsEnable();

	// Возвращает тип контроллера секущей (VSN::Controller::ControllerViews).
	VSN::Controller::ControllerViews ControllerType();

public slots:
	// Создает секущую.
	void CreateCutingTools();

	// Изменяет направление секущей.
	void ChangeCuttingDirection();

	// Изменяет способ навигации секущей.
	void ChengeNavigationOfCuttingTool();

	// Изменяет статус включения/выключения секущей.
	void ChangeEnableOfCuttingTool();

	// Изменяет индекс активной секущей.
	void changeCurrentCuttingToolIndex(int index);
};
#endif // !_CUTTING_TOOLS_


