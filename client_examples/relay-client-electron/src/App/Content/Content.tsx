import React from 'react';
import TabView from '../Components/TabView/TabView';
import { Tab } from '../Models/Tab';
import TabContent from '../Components/TabView/TabContent/TabContent';
import ControlPanel from './ControlPanel/ControlPanel';
import DeviceConfiguration from './DeviceConfiguration/DeviceConfiguration';

const Tabs: Tab[] = [
    {
        name: 'Control panel',
    },
    {
        name: 'Device configuration',
    },
];

export default function Content() {
    return (
        <TabView tabs={Tabs}>
            <>
                <TabContent tabName="Control panel">
                    <ControlPanel />
                </TabContent>
                <TabContent tabName="Device configuration">
                    <DeviceConfiguration />
                </TabContent>
            </>
        </TabView>
    );
}
