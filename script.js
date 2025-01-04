const docs = {
    navigation: [
        { id: 'getting-started', title: 'Getting Started', file: 'docs/getting-started.md' },
        { id: 'installation', title: 'Installation', file: 'docs/installation.md' },
        { id: 'configuration', title: 'Configuration', file: 'docs/configuration.md' },
        // Add more documentation pages here
    ]
};

// Initialize the documentation site
function initDocs() {
    createNavigation();
    loadContent(window.location.hash || '#getting-started');

    // Handle navigation
    window.addEventListener('hashchange', () => {
        loadContent(window.location.hash);
    });
}

// Create navigation menu
function createNavigation() {
    const navMenu = document.getElementById('nav-menu');
    docs.navigation.forEach(item => {
        const link = document.createElement('a');
        link.href = `#${item.id}`;
        link.textContent = item.title;
        navMenu.appendChild(link);
    });
}

// Load content from markdown files
async function loadContent(hash) {
    const id = hash.replace('#', '');
    const navItem = docs.navigation.find(item => item.id === id);
    
    if (!navItem) {
        loadContent('#getting-started');
        return;
    }

    try {
        const response = await fetch(navItem.file);
        const markdown = await response.text();
        const contentArea = document.getElementById('content-area');
        contentArea.innerHTML = marked.parse(markdown);

        // Update active navigation item
        document.querySelectorAll('#nav-menu a').forEach(link => {
            link.classList.toggle('active', link.getAttribute('href') === hash);
        });

        // Update document title
        document.title = `${navItem.title} - Documentation`;
    } catch (error) {
        console.error('Error loading content:', error);
    }
}

// Initialize when the page loads
document.addEventListener('DOMContentLoaded', initDocs);

// Example markdown file: docs/getting-started.md
# Getting Started

Welcome to our documentation! This guide will help you get started with our product.

## Prerequisites

Before you begin, make sure you have:
- Basic understanding of HTML and CSS
- A text editor
- A web browser
- Git installed (for version control)

## Quick Start

1. Clone the repository:
\`\`\`bash
git clone https://github.com/yourusername/yourproject.git
\`\`\`

2. Open \`index.html\` in your browser to view the documentation.

3. Start editing the markdown files in the \`docs\` folder to update the content.

// Example markdown file: docs/installation.md
# Installation Guide

This guide provides detailed instructions for installing and configuring our product.

## System Requirements

- Modern web browser (Chrome, Firefox, Safari, or Edge)
- Text editor (VS Code, Sublime Text, or similar)
- Basic command line knowledge

## Installation Steps

1. Download the latest release from our GitHub repository
2. Extract the files to your desired location
3. Open the project in your text editor
4. Customize the configuration as needed

## Configuration

The basic configuration can be found in \`config.js\`. Here's an example:

\`\`\`javascript
{
  "theme": "light",
  "language": "en",
  "showSidebar": true
}
\`\`\`
