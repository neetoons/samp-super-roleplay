module.exports = (sequelize, DataTypes) => {
    return sequelize.define("crew_ranks",
        {
            id: {
                type: DataTypes.INTEGER,
                allowNull: false,
                autoIncrement: true,
                primaryKey: true
            },
            rank_pos: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            rank_name: {
                type: DataTypes.STRING(24),
                allowNull: false
            },
            permission0: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            permission1: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            permission2: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            permission3: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            permission4: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            permission5: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            permission6: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            permission7: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            permission8: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            permission9: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            },
            permission10: {
                type: DataTypes.INTEGER,
                allowNull: false,
                defaultValue: 0
            }
        },
        {
            freezeTableName: true,
            timestamps: false
        }
    );
};