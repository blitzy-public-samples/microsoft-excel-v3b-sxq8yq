import React, { useState } from 'react';
import { useDispatch, useSelector } from 'react-redux';
import styled from '@emotion/styled';
import RibbonTab from './RibbonTab';
import RibbonGroup from './RibbonGroup';
import RibbonButton from './RibbonButton';
import { setActiveTab } from '../store/ribbonSlice';
import { RootState } from '../store/types';

// Define the props interface for the Ribbon component
interface RibbonProps {
  workbookId: string;
}

// Define styled components for the Ribbon
const RibbonContainer = styled.div`
  display: flex;
  flex-direction: column;
  background-color: #f3f3f3;
  border-bottom: 1px solid #d1d1d1;
`;

const RibbonTabs = styled.div`
  display: flex;
  background-color: #ffffff;
`;

const RibbonContent = styled.div`
  padding: 4px 8px;
`;

// Define the constant for ribbon tabs
const RIBBON_TABS = [
  'Home',
  'Insert',
  'Page Layout',
  'Formulas',
  'Data',
  'Review',
  'View',
  'Help'
];

// Main Ribbon component
const Ribbon: React.FC<RibbonProps> = ({ workbookId }) => {
  const dispatch = useDispatch();
  const activeTab = useSelector((state: RootState) => state.ribbon.activeTab);

  // Handle tab click events
  const handleTabClick = (tabName: string) => {
    dispatch(setActiveTab(tabName));
  };

  return (
    <RibbonContainer>
      <RibbonTabs>
        {RIBBON_TABS.map((tab) => (
          <RibbonTab
            key={tab}
            label={tab}
            active={activeTab === tab}
            onClick={() => handleTabClick(tab)}
          />
        ))}
      </RibbonTabs>
      <RibbonContent>
        {/* Render content based on active tab */}
        {activeTab === 'Home' && (
          <>
            <RibbonGroup title="Clipboard">
              <RibbonButton label="Paste" />
              <RibbonButton label="Cut" />
              <RibbonButton label="Copy" />
            </RibbonGroup>
            <RibbonGroup title="Font">
              <RibbonButton label="Bold" />
              <RibbonButton label="Italic" />
              <RibbonButton label="Underline" />
            </RibbonGroup>
            {/* Add more RibbonGroups and RibbonButtons for the Home tab */}
          </>
        )}
        {/* Add conditional rendering for other tabs */}
      </RibbonContent>
    </RibbonContainer>
  );
};

export default Ribbon;

// Human tasks:
// TODO: Implement specific functionality for each ribbon tab
// TODO: Add icons and tooltips to ribbon buttons
// TODO: Implement responsive design for smaller screen sizes
// TODO: Add keyboard shortcuts for ribbon actions
// TODO: Implement customizable quick access toolbar
// TODO: Add localization support for ribbon labels