import React from 'react';

export default function TabContent({
    children,
    tabId,
}: {
    children: React.ReactNode | React.ReactNode[];
    tabId: string;
}) {
    return <>{children}</>;
}

TabContent.isTabContent = true;
