import React from 'react';
import { Tab } from '../../../Models/Tab';
import './tab-selector.scss';
import clsx from 'clsx';

export type TabCallback = (tabName: string) => void;

export default function TabSelector({
    tabs,
    selectedTab,
    tabClick,
}: {
    tabs: Tab[];
    selectedTab: string;
    tabClick: TabCallback;
}) {
    const onTabClick = (name: string) => tabClick(name);

    return (
        <div className="tabSelector">
            {tabs.map((tab, i) => (
                <a
                    href="#"
                    role="button"
                    onClick={() => onTabClick(tab.name)}
                    key={i}
                >
                    <div
                        className={clsx(
                            'tabSelector--tab',
                            tab.name === selectedTab ? 'active' : ''
                        )}
                    >
                        {tab.name}
                    </div>
                </a>
            ))}
        </div>
    );
}
