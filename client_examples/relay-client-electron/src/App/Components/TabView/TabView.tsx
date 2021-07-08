import React, { ReactNode, useState } from 'react';
import TabSelector from './TabSelector/TabSelector';
import { Tab } from '../../Models/Tab';
import './tab-view.scss';

type NodeMap = { [key: string]: ReactNode };

export default function TabView({
    children,
    tabs,
    initialSelection,
}: {
    children: ReactNode;
    tabs: Tab[];
    initialSelection?: string;
}) {
    const [selectedTab, setSelectedTab] = useState(
        initialSelection || tabs[0].name
    );

    // If the direct child is not a TabContent item, it's probably a wrapper
    // for more children, so we retrieve those children instead
    if (
        !(children as any).type.isTabContent &&
        (children as any).props &&
        (children as any).props.children
    ) {
        children = (children as any).props.children;
    }

    let content: NodeMap = {};
    content = (children as ReactNode[]).reduce<NodeMap>(
        (obj: NodeMap, node: ReactNode) => {
            const nodeObj = node as any;
            if (nodeObj.type.isTabContent) {
                return {
                    ...obj,
                    [nodeObj.props.tabName]: node,
                };
            } else return obj;
        },
        {}
    );

    const tabChanged = (name: string) => {
        setSelectedTab(name);
    };

    return (
        <>
            <TabSelector
                tabs={tabs}
                tabClick={(name) => tabChanged(name)}
                selectedTab={selectedTab}
            />
            <div className="tabContent">{content[selectedTab]}</div>
        </>
    );
}