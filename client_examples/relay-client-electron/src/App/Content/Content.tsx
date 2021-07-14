import React from 'react';
import TabView from '../Components/TabView/TabView';
import { Tab } from '../Models/Tab';
import TabContent from '../Components/TabView/TabContent/TabContent';
import ControlPanel from './ControlPanel/ControlPanel';
import DeviceConfiguration from './DeviceConfiguration/DeviceConfiguration';
import AppSettings from './AppSettings/AppSettings';

const Tabs: Tab[] = [
    {
        name: 'Control panel',
        id: 'control_panel',
    },
    {
        name: 'Device configuration',
        id: 'device_config',
    },
    {
        name: 'Settings',
        id: 'settings',
    },
];

export default function Content() {
    return (
        <TabView tabs={Tabs}>
            <>
                <TabContent tabId="control_panel">
                    <ControlPanel />
                </TabContent>
                <TabContent tabId="device_config">
                    <DeviceConfiguration />
                </TabContent>
                <TabContent tabId="settings">
                    <AppSettings />
                </TabContent>
            </>
        </TabView>
    );
}
