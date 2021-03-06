/*
                          qmusic

    Copyright (C) 2015 Arthur Benilov,
    arthur.benilov@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
*/

#ifndef INPUTPORT_H
#define INPUTPORT_H

#include "Port.h"
#include "FrameworkApi.h"

class OutputPort;

/**
 * @brief Input port.
 *
 * Incoming signal port.
 * Input port holds a pointer to connected output port (if any).
 * The data (value) provided by the input port comed from
 * connected output port.
 *
 * Input port does not perform any caching of sugnal data.
 *
 * @see OutputPort
 */
class QMUSIC_FRAMEWORK_API InputPort : public Port
{
public:

    /// Default constructor.
    InputPort();

    /**
     * Construct an input port.
     * @param name Port name.
     * @param defaultValue Default value reported by disconnected input.
     */
    InputPort(const QString &name, float defaultValue = 0.0f);

    /**
     * Returns port value.
     * Value is taked from connected output port. If not connected an invalid value is returned.
     * @return
     */
    float value() const override;

    /**
     * Fast access to the port value.
     * @return
     */
    inline float getValue() const { return *m_pValue; }

    /**
     * Update the input.
     * This will trigger update of connected output port (if any).
     */
    void update() override;

    /**
     * Returns this input port index (within the list of all inut ports of this audio unit).
     * @return
     */
    int index() const override final;

    /**
     * Returns port's default value.
     * Default value is used when port is not connected to any output.
     * @return Default value.
     */
    float defaultValue() const { return m_defaultValue; }

    /**
     * Assign port default value.
     * @param v Value to be set.
     */
    void setDefaultValue(float v) { m_defaultValue = v; }

    /**
     * Connect to an output port.
     * @param pOutput Pointer to the output port to connect to.
     */
    void connect(OutputPort *pOutput);

    /**
     * Disconnect this port from the output.
     */
    void disconnect();

    /**
     * Returns pointer to connected outpu port.
     * @return
     */
    OutputPort* connectedOutputPort() const { return m_pConnectedOutputPort; }

private:

    /// Pointer to connected output port, if any.
    OutputPort *m_pConnectedOutputPort;

    /**
     * Pointer to the value holder.
     * This will point to the connected output port value holder
     * or to the default value of this port if not connected.
     */
    const float *m_pValue;

    /// Default value used when port is not connected.
    float m_defaultValue;
};

#endif // INPUTPORT_H

