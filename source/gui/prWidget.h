// File: prWidget.h
/**
 * Copyright 2014 Paul Michael McNab
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __PRWIDGET_H
#define __PRWIDGET_H


#include "../prConfig.h"
#include "../core/prTypes.h"
#include "../core/prString.h"
#include "../input/prTouch.h"
#include "../math/prVector2.h"
#include "../display/prColour.h"


// Enum: prWidgetType
//      GUI widget types.
//
// - WT_Button
// - WT_Dialog
//
typedef enum
{
    WT_Button,
    WT_Dialog,

} prWidgetType;


// Forward declarations 
class prSpriteManager;


// Class: prWidget
//      Base GUI widget class
class prWidget
{
public:
    // Method: prWidget
    //      Ctor
    prWidget(prWidgetType type, const char *name, prSpriteManager *pSpriteManager);

    // Method: ~prWidget
    //      Dtor
    virtual ~prWidget();

    // Method: Name
    //      Get the widget name.
    const char *Name() const { return m_name.Text(); }

    // Method: Type
    //      Returns the widget type.
    //
    // See Also:
    //      <prWidgetType>
    s32 Type() const { return m_type; }

    // Method: SetVisible
    //      Set the render visibility state.
    void SetVisible(bool state) { m_visible = state; }

    // Method: GetVisible
    //      Get the render visibility state.
    bool GetVisible() const     { return m_visible; }

    // Method: SetEnabled
    //      Sets the enabled state. If disabled, a widget should look disabled.
    void SetEnabled(bool state) { m_enabled = state; }

    // Method: GetEnabled
    //      Gets the enabled state. If disabled, a widget should look disabled.
    bool GetEnabled() const     { return m_enabled; }
    
    // Method: SetActive
    //      If acive a widget will be updated. Meant for modal dialogs.
    void SetActive(bool state) { m_active = state; }

    // Method: GetActive
    //      If acive a widget will be updated. Meant for modal dialogs.
    bool GetActive() const { return m_active; }
    
    // Method: SetAnimated
    //      Sets the animated state.
    //
    // Notes:
    //      Used with sprite based widgets
    void SetAnimated(bool state) { m_animated = state; }
    
    // Method: GetAnimated
    //      Gets the animated state.
    //
    // Notes:
    //      Used with sprite based widgets
    bool GetAnimated() const    { return m_animated; }

    // Method: GetDestroy
    //      Is this widget to be destroyed?
    bool GetDestroy() const { return m_destroy; }

    // Method: SetDestroy
    //      Tells the GUI manager to destroy this widget
    void SetDestroy() { m_destroy = true; }

    // Method: SetColour
    //      Sets the widget colour.
    //
    // Parameters:
    //      c - The colour
    void SetColour(prColour c) { m_colour = c; }

    // Method: SetLayer
    //      Sets the widget layer.
    //
    // Parameters:
    //      layer - The layer
    void SetLayer(s32 layer) { m_layer = layer; }

    // Method: GetLayer
    //      Gets the widget layer.
    s32 GetLayer() const { return m_layer; }

    // Method: Update
    //      Updates the widget
    //
    // Parameters:
    //      dt - Delta time
    virtual void Update(f32 dt) = 0;

    // Method: Draw
    //      Draws the widget
    virtual void Draw() = 0;

    // Method: OnPressed
    //      A touch event handler
    //
    // Parameters:
    //      e - A touch event
    //
    // See Also:
    //      <prTouchEvent>
    virtual void OnPressed(const prTouchEvent &e) = 0;

    // Method: OnMove
    //      A touch event handler
    //
    // Parameters:
    //      e - A touch event
    //
    // See Also:
    //      <prTouchEvent>
    virtual void OnMove(const prTouchEvent &e) = 0;

    // Method: OnReleased
    //      A touch event handler
    //
    // Parameters:
    //      e - A touch event
    //
    // See Also:
    //      <prTouchEvent>
    virtual void OnReleased(const prTouchEvent &e) = 0;


private:
    prWidgetType    m_type;
    prString        m_name;


protected:
    bool                m_visible;
    bool                m_enabled;
    bool                m_animated;
    bool                m_active;
    bool                m_destroy;
    bool                m_exp0;
    bool                m_exp1;
    bool                m_exp2;
    prSpriteManager    *m_pSpriteManager;
    prColour            m_colour;
    s32                 m_layer;


public:
    prVector2   pos;
};


#endif//__PRWIDGET_H
