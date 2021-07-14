import React from 'react';
import { Tab } from '../../../Models/Tab';
import './tab-selector.scss';
import clsx from 'clsx';

export type TabCallback = (tabId: string) => void;

export default function TabSelector({
    tabs,
    selectedTab,
    tabClick,
}: {
    tabs: Tab[];
    selectedTab: string;
    tabClick: TabCallback;
}) {
    const onTabClick = (id: string) => tabClick(id);

    return (
        <div className="tabSelector">
            {tabs.map((tab, i) => (
                <a
                    href="#"
                    role="button"
                    onClick={() => onTabClick(tab.id)}
                    key={i}
                >
                    <div
                        className={clsx(
                            'tabSelector--tab',
                            tab.id === selectedTab ? 'active' : ''
                        )}
                    >
                        {tab.name}
                    </div>
                </a>
            ))}
        </div>
    );
}
