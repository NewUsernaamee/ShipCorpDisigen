#ifndef VISION_3D_VIEW
#define VISION_3D_VIEW
 


#include <vsn_application.h>
#include <qt_openglwidget.h>
#include <qt_openglwindow.h>
#include <last.h>
#include <ShipModel.h>
#include <vector>
#include "templ_sptr.h"
#include "curve.h"
#include <action_curve3d.h>
#include "action_curve.h"
#include "templ_s_array.h"
#include "mb_placement3d.h"
#include <cmath>
#include <cur_offset_curve.h>
#include <curve3d.h>
#include <mb_cube.h>
#include <cur_plane_curve.h>
#include <space_instance.h>
#include <action_surface.h>
#include <action_solid.h>
#include <action_shell.h>
#include <cur_contour.h>
#include <mb_placement.h>
#include <qmessagebox.h>
#include <cur_polyline3d.h>
#include <conv_model_exchange.h>
#include <vsn_renderobject.h>
#include <utility>
#include <CuttingTools.h>

class Vision3DView: public QtVision::QtOpenGLSceneWindow
{
public:
    Vision3DView(QWindow* parent = nullptr);
    
    // Добавляет сегмент в сцену.
    void addSceneSegment(GeometryRep* pShapeRep, const MbVector3D& vecMove, const Color& color);

    // Синхронизироваться с моделью корабля.
    void synchronizeWithModel(ShipModel* model);

    // Добавляет твердое тело в сцену.
    void addSolidToScene(ShipModel* model);

    // Ставит флаг m_isVisible в положение true, виджет понимает что его видно
    void nowVisible();

    // Ставит флаг m_isVisible в положение false, виджет понимает что его не видно
    void nowInvisible();

    // позволяет отображать твёрдое тело
    void displaySolid();

    // позволяет отображать поверхность
    void displaySurface();

    // Возвращает указатель на инструменты резки.
    CuttingTools* GetCuttingTools();

private:
    std::vector<NodeKey> m_segmentKeys;
    bool m_isVisible;
    bool m_isSurface;
    CuttingTools* m_cuttingTools = nullptr;

    // Создает поверхность на основе модели корабля.
    MbSurface* CreateSurfaseByModel(ShipModel* model);

    // Создает твердое тело на основе модели корабля.
    c3d::SolidSPtr CreateSolidByModel(ShipModel* model);

public slots:
    // Устанавливает количество режущих поверхностей.
    void setCuttingCount(size_t count);

    // Обновляет сцену на основе модели корабля.
    void Refresh(ShipModel* model);
};
#endif //VISION_3D_VIEW