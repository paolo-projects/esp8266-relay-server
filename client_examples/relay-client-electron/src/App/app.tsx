import React from 'react';
import Header from './Header/Header';
import Content from './Content/Content';

declare var window: {
    ipc: any;
};

export default function App() {
    console.log('ipc is', window.ipc);

    return (
        <>
            <Header />
            <Content />
        </>
    );
}
