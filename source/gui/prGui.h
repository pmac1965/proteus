// File: prGui.h
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


#ifndef __PRGUI_H
#define __PRGUI_H


#include "prWidget.h"
#include "../input/prTouchListener.h"
#include "../display/prSpriteManager.h"
#include <list>


// Class: prGui
//      Container for GUI widgets
class prGui : public ITouchListener
{
public:
    // Method: prGui
    //      Ctor
    prGui();

    // Method: ~prGui
    //      Dtor
    ~prGui();

    // Method: Create
    //      Creates a widget
    //
    // Parameters:
    //      type - The type of widget to create
    //      name - The name of this widget. Should be unique
    //
    // See Also:
    //      <prWidgetType>
    prWidget *Create(prWidgetType type, const char *name);

    // Method: Update
    //      Update the GUI
    void Update(f32 dt);

    // Method: Draw
    //      Draw the GUI widgets
    void Draw();

    // Method: Clear
    //      Clear the GUI of all widgets
    void Clear();

    // Method: SetEnable
    //      Set the GUI's enabled state
    void SetEnable(PRBOOL state) { m_enabled = state; }

    // Method: SetVisible
    //      Set the GUI's visible state
    void SetVisible(PRBOOL state) { m_visible = state; }

    // Method: IsEnabled
    //      Is the GUI enabled?
    PRBOOL IsEnabled() const { return m_enabled; }

    // Method: IsVisible
    //      Is the GUI visible?
    PRBOOL IsVisible() const { return m_enabled; }

    // Method: Find
    //      Finds a GUI item
    //
    // Parameters:
    //      name - The name of the item
    prWidget *Find(const char *name);

    // Method: GetWidgetCount
    //      Returns the number of widgets
    s32 GetWidgetCount() const { return (s32)m_widgets.size(); }

    // Method: SetBaseLayer
    //      Sets the base layer for GUI dialogs
    //
    // Notes:
    //      This is useful for when your using multiple GUI's as it stops
    //      them mixing messages
    void SetBaseLayer(u32 layer) { m_layer     = layer;
                                   m_baseLayer = layer; }

    // Method: InputPressed
    //      Passes on a touch event to the GUI items
    //
    // Parameters:
    //      e - A touch event
    //
    // See Also:
    //      <prTouchEvent>
    void InputPressed(const prTouchEvent &e);

    // Method: InputReleased
    //      Passes on a touch event to the GUI items
    //
    // Parameters:
    //      e - A touch event
    //
    // See Also:
    //      <prTouchEvent>
    void InputReleased(const prTouchEvent &e);

    // Method: InputAxis
    //      Passes on a touch event to the GUI items
    //
    // Parameters:
    //      e - A touch event
    //
    // See Also:
    //      <prTouchEvent>
    void InputAxis(const prTouchEvent &e);


private:

    std::list<prWidget *>   m_widgets;
    prSpriteManager         m_spriteManager;
    PRBOOL                  m_enabled;
    PRBOOL                  m_visible;
    s32                     m_layer;
    s32                     m_baseLayer;
};


#endif//__PRGUI_H
