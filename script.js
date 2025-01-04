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
