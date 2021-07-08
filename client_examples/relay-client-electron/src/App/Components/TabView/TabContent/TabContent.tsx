import React from 'react';

export default function TabContent({
    children,
    tabName,
}: {
    children: React.ReactNode | React.ReactNode[];
    tabName: string;
}) {
    return <>{children}</>;
}

TabContent.isTabContent = true;
